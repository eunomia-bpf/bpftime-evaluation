#ifndef __x86_64__
#error "This poc only works on x86_64"
#endif
#include "ebpf-core.h"
#include "ebpf_vm.h"
#include <algorithm>
#include <asm-generic/errno-base.h>
#include <asm/unistd_64.h>
#include <cerrno>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <ostream>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>
#include <iostream>
#include <unordered_map>
#include <linux/bpf.h>
#include <memory>
#include <utility>
#include <variant>
#include <linux/perf_event.h>
#include <unistd.h>
#include "helpers_impl.hpp"
#include <vector>
#include <linux/mman.h>
#include "wrapper.hpp"
using syscall_fn = long (*)(long, ...);
using close_fn = int (*)(int);
using mmap64_fn = void *(*)(void *, size_t, int, int, int, off64_t);
static syscall_fn orig_syscall_fn = nullptr;
static close_fn orig_close_fn = nullptr;
static mmap64_fn orig_mmap64_fn = nullptr;
static int handle_close(int);
static long handle_sysbpf(int cmd, union bpf_attr *attr, size_t size);
static int handle_perfevent(perf_event_attr *attr, pid_t pid, int cpu,
			    int group_fd, unsigned long flags);
static void *handle_mmap64(void *addr, size_t length, int prot, int flags,
			   int fd, off_t offset);

extern "C" uint64_t map_ptr_by_fd(uint32_t fd);

extern "C" void *mmap64(void *addr, size_t length, int prot, int flags, int fd,
			off64_t offset)
{
	if (orig_mmap64_fn == nullptr) {
		orig_mmap64_fn = (mmap64_fn)dlsym(RTLD_NEXT, "mmap");
	}
	std::cerr << "Mmap64 " << addr << std::endl;
	return handle_mmap64(addr, length, prot, flags, fd, offset);
}
extern "C" int close(int fd)
{
	if (orig_close_fn == nullptr) {
		orig_close_fn = (close_fn)dlsym(RTLD_NEXT, "close");
	}
	std::cout << "Closing " << fd << std::endl;
	return handle_close(fd);
}
extern "C" long syscall(long sysno, ...)
{
	if (orig_syscall_fn == nullptr) {
		orig_syscall_fn = (syscall_fn)dlsym(RTLD_NEXT, "syscall");
	}
	// glibc directly reads the arguments without considering
	// the underlying argument number. So did us
	va_list args;
	va_start(args, sysno);
	long arg1 = va_arg(args, long);
	long arg2 = va_arg(args, long);
	long arg3 = va_arg(args, long);
	long arg4 = va_arg(args, long);
	long arg5 = va_arg(args, long);
	long arg6 = va_arg(args, long);
	va_end(args);
	if (sysno == __NR_bpf) {
		std::cout << "SYS_BPF"
			  << " " << arg1 << " " << arg2 << " " << arg3 << " "
			  << arg4 << " " << arg5 << " " << arg6 << std::endl;
		int cmd = (int)arg1;
		auto attr = (union bpf_attr *)(uintptr_t)arg2;
		auto size = (size_t)arg3;
		return handle_sysbpf(cmd, attr, size);
	} else if (sysno == __NR_perf_event_open) {
		std::cout << "SYS_PERF_EVENT_OPEN"
			  << " " << arg1 << " " << arg2 << " " << arg3 << " "
			  << arg4 << " " << arg5 << " " << arg6 << std::endl;
		return handle_perfevent((perf_event_attr *)(uintptr_t)arg1,
					(pid_t)arg2, (int)arg3, (int)arg4,
					(unsigned long)arg5);
	} else if (sysno == __NR_ioctl) {
		std::cout << "SYS_IOCTL"
			  << " " << arg1 << " " << arg2 << " " << arg3 << " "
			  << arg4 << " " << arg5 << " " << arg6 << std::endl;
	}
	return orig_syscall_fn(sysno, arg1, arg2, arg3, arg4, arg5, arg6);
}

std::unordered_map<int, std::unique_ptr<EbpfObj> > objs;
static int next_id = 1 << 20;

static int handle_close(int fd)
{
	if (auto itr = objs.find(fd); itr != objs.end()) {
		std::cerr << "Destroy ebpf obj: " << fd << " type "
			  << name_mapping[itr->second->index()] << std::endl;
		objs.erase(itr);
		return 0;
	} else {
		return orig_close_fn(fd);
	}
}

static long handle_sysbpf(int cmd, union bpf_attr *attr, size_t size)
{
	char *errmsg;
	switch (cmd) {
	case BPF_PROG_LOAD:
		// Load a program?
		{
			std::cerr << "Loading program `" << attr->prog_name
				  << "` license `"
				  << (const char *)(uintptr_t)attr->license
				  << "`" << std::endl;
			EbpfProgWrapper prog;

			if (int err = ebpf_load(prog.vm.get(),
						(void *)(uintptr_t)attr->insns,
						(uint32_t)attr->insn_cnt * 8,
						&errmsg);
			    err < 0) {
				std::cerr << "Failed to load code into vm, err="
					  << err << " message="
					  << (errmsg == nullptr ? "<unknown>" :
								  errmsg)
					  << std::endl;
				if (errmsg)
					free(errmsg);
				errno = -EINVAL;
				return -1;
			}
			auto id = next_id++;
			objs.emplace(
				id, std::make_unique<EbpfObj>(std::move(prog)));
			std::cerr << "Loaded program `" << attr->prog_name
				  << "` id =" << id << std::endl;
			return id;
		}
	case BPF_LINK_CREATE: {
		auto prog_fd = attr->link_create.prog_fd;
		auto target_fd = attr->link_create.target_fd;
		if (auto itr = objs.find(prog_fd); itr != objs.end()) {
			if (!std::holds_alternative<EbpfProgWrapper>(
				    *itr->second)) {
				std::cerr << "prog fd " << prog_fd
					  << " is not program" << std::endl;
				errno = -EINVAL;
				return -1;
			}
		} else {
			std::cerr << "No prog fd " << prog_fd << " found"
				  << std::endl;
			errno = -ENOENT;
			return -1;
		}
		if (auto itr = objs.find(target_fd); itr != objs.end()) {
			if (!std::holds_alternative<PerfEventWrapper>(
				    *itr->second)) {
				std::cerr << "target fd " << target_fd
					  << " is not perf event" << std::endl;
				errno = -EINVAL;
				return -1;
			}
		} else {
			std::cerr << "No perf event fd " << target_fd
				  << " found" << std::endl;
			errno = -ENOENT;
			return -1;
		}
		auto id = next_id++;
		objs.emplace(id, std::make_unique<EbpfObj>(
					 BpfLinkWrapper(prog_fd, target_fd)));
		return id;
	}
	case BPF_MAP_CREATE: {
		EbpfMapWrapper map((enum bpf_map_type)attr->map_type,
				   (uint32_t)attr->key_size,
				   (uint32_t)attr->value_size,
				   (uint32_t)attr->max_entries,
				   (uint64_t)attr->flags, attr->map_name);
		auto id = next_id++;
		objs.emplace(id, std::make_unique<EbpfObj>(std::move(map)));
		return id;
	}
	case BPF_MAP_UPDATE_ELEM: {
		if (auto itr = objs.find(attr->map_fd); itr != objs.end()) {
			if (std::holds_alternative<EbpfMapWrapper>(
				    *itr->second)) {
				auto &map =
					std::get<EbpfMapWrapper>(*itr->second);
				int err = map.mapUpdate(
					(const void *)(uintptr_t)(attr->key),
					(const void *)(uintptr_t)attr->value,
					(uint64_t)attr->flags);
				if (err < 0) {
					errno = err;
					return -1;
				}
				return err;
			} else {
				errno = -EINVAL;
				return -1;
			}
		} else {
			errno = -ENOENT;
			return -1;
		}
	}
	case BPF_MAP_FREEZE: {
		if (auto itr = objs.find(attr->map_fd); itr != objs.end()) {
			if (std::holds_alternative<EbpfMapWrapper>(
				    *itr->second)) {
				auto &map =
					std::get<EbpfMapWrapper>(*itr->second);
				map.frozen = true;
				return 0;
			} else {
				errno = -EINVAL;
				return -1;
			}
		} else {
			errno = -ENOENT;
			return -1;
		}
	}
	default:
		return orig_syscall_fn(__NR_bpf, (long)cmd,
				       (long)(uintptr_t)attr, (long)size);
	};
	return 0;
}

static int handle_perfevent(perf_event_attr *attr, pid_t pid, int cpu,
			    int group_fd, unsigned long flags)
{
	if (attr->type == PERF_TYPE_TRACEPOINT) {
		auto id = next_id++;

		objs.emplace(id, std::make_unique<EbpfObj>(PerfEventWrapper()));
		return id;
	} else {
		return orig_syscall_fn(__NR_perf_event_open,
				       (uint64_t)(uintptr_t)attr, (uint64_t)pid,
				       (uint64_t)cpu, (uint64_t)group_fd,
				       (uint64_t)flags);
	}
	return 0;
}

static void *handle_mmap64(void *addr, size_t length, int prot, int flags,
			   int fd, off64_t offset)
{
	if (auto itr = objs.find(fd); itr != objs.end()) {
		if (std::holds_alternative<EbpfMapWrapper>(*itr->second)) {
			auto ptr = (uint8_t *)orig_mmap64_fn(
				addr, length, prot | PROT_WRITE,
				flags | MAP_ANONYMOUS, -1, 0);
			auto &wrapper = std::get<EbpfMapWrapper>(*itr->second);
			auto &map = std::get<ArrayMapImpl>(wrapper.impl);
			for (size_t i = 0; i < map.size(); i++) {
				std::copy(map[i].begin(), map[i].end(),
					  ptr + i * wrapper.value_size);
			}
			return (void *)ptr;
		}
	}
	return orig_mmap64_fn(addr, length, prot, flags, fd, offset);
}
