#include <asm-generic/errno-base.h>
#include <bits/types/struct_timespec.h>
#include <cstring>
#include <ctime>
#include <ebpf-core.h>
#include <cinttypes>
#include <functional>
#include <optional>
#include <ostream>
#include <unistd.h>
#include "wrapper.hpp"
#include <variant>
extern std::unordered_map<int, std::unique_ptr<EbpfObj> > objs;

extern "C" uint64_t map_ptr_by_fd(uint32_t fd)
{
	if (auto itr = objs.find((int)fd); itr != objs.end()) {
		if (std::holds_alternative<EbpfMapWrapper>(*itr->second)) {
			// Use a convenient way to represent a pointer
			return ((uint64_t)fd << 32) | 0xffffffff;
		} else {
			std::cerr << "fd " << fd << " does not point to a map"
				  << std::endl;
			return 0;
		}
	} else {
		std::cerr << "map fd " << fd << " does not exist" << std::endl;
		return 0;
	}
}

extern "C" uint64_t map_val(uint64_t map_ptr)
{
	if (auto itr = objs.find((int)(map_ptr >> 32)); itr != objs.end()) {
		if (std::holds_alternative<EbpfMapWrapper>(*itr->second)) {
			auto &map = std::get<EbpfMapWrapper>(*itr->second);
			return (uint64_t)(uintptr_t)map.first_value_addr();
		} else {
			std::cerr << "Invalid map ptr " << map_ptr << std::endl;
			return 0;
		}
	} else {
		std::cerr << "Invalid map ptr " << map_ptr << std::endl;
		return 0;
	}
}

extern "C" uint64_t bpf_probe_read(uint64_t dst, uint64_t size, uint64_t ptr,
				   uint64_t, uint64_t)
{
	memcpy((void *)(uintptr_t)dst, (void *)(uintptr_t)ptr,
	       (size_t)(uint32_t)(size));
	return 0;
}

extern "C" uint64_t bpf_ktime_get_coarse_ns(uint64_t, uint64_t, uint64_t,
					    uint64_t, uint64_t)
{
	timespec spec;
	clock_gettime(CLOCK_MONOTONIC_COARSE, &spec);
	return spec.tv_sec * (uint64_t)1000000000 + spec.tv_nsec;
}

extern "C" uint64_t bpf_get_current_pid_tgid(uint64_t, uint64_t, uint64_t,
					     uint64_t, uint64_t)
{
	return ((uint64_t)getgid() << 32) | getpid();
}

extern "C" uint64_t bpf_ktime_get_ns(uint64_t, uint64_t, uint64_t, uint64_t,
				     uint64_t)
{
	timespec spec;
	clock_gettime(CLOCK_MONOTONIC, &spec);
	return spec.tv_sec * (uint64_t)1000000000 + spec.tv_nsec;
}

extern "C" uint64_t bpf_map_lookup_elem(uint64_t map, uint64_t key, uint64_t,
					uint64_t, uint64_t)
{
	if (auto itr = objs.find((int)(map >> 32)); itr != objs.end()) {
		if (std::holds_alternative<EbpfMapWrapper>(*itr->second)) {
			auto &map = std::get<EbpfMapWrapper>(*itr->second);
			return (uint64_t)(uintptr_t)map.mapLookup(
				(const void *)(uintptr_t)key);
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}
extern "C" uint64_t bpf_map_update_elem(uint64_t map, uint64_t key,
					uint64_t value, uint64_t flags,
					uint64_t)
{
	if (auto itr = objs.find((int)(map >> 32)); itr != objs.end()) {
		if (std::holds_alternative<EbpfMapWrapper>(*itr->second)) {
			auto &map = std::get<EbpfMapWrapper>(*itr->second);
			return map.mapUpdate((const void *)(uintptr_t)key,
					     (const void *)(uintptr_t)value,
					     flags);
		} else {
			return -EINVAL;
		}
	} else {
		return -ENOENT;
	}
}

extern "C" uint64_t bpf_map_delete_elem(uint64_t map, uint64_t key, uint64_t,
					uint64_t, uint64_t)
{
	if (auto itr = objs.find((int)(map >> 32)); itr != objs.end()) {
		if (std::holds_alternative<EbpfMapWrapper>(*itr->second)) {
			auto &map = std::get<EbpfMapWrapper>(*itr->second);
			return map.mapDelete((const void *)(uintptr_t)key);
		} else {
			return -EINVAL;
		}
	} else {
		return -ENOENT;
	}
}
static inline uint64_t
visit_ringbuf_impl(int fd, std::function<uint64_t(RingBuffer &)> f,
		   bool use_neg_ret)
{
	if (auto itr = objs.find(fd); itr != objs.end()) {
		if (std::holds_alternative<EbpfMapWrapper>(*itr->second)) {
			auto &map = std::get<EbpfMapWrapper>(*itr->second);
			if (std::holds_alternative<RingBufMapImpl>(map.impl)) {
				auto &rb = std::get<RingBufMapImpl>(map.impl);
				return f(*rb);
			} else
				return use_neg_ret ? -EINVAL : 0;
		} else {
			return use_neg_ret ? -EINVAL : 0;
		}
	} else {
		return use_neg_ret ? -ENOENT : 0;
	}
}
extern "C" uint64_t bpf_ringbuf_reserve(uint64_t ringbuf, uint64_t size,
					uint64_t flags, uint64_t, uint64_t)
{
	int fd = ringbuf >> 32;
	return visit_ringbuf_impl(
		fd,
		[=](auto &v) -> uint64_t {
			return (uint64_t)(uintptr_t)v.reserve(size, fd);
		},
		false);
}

extern "C" uint64_t bpf_get_current_task(uint64_t, uint64_t, uint64_t, uint64_t,
					 uint64_t)
{
}

#define REGISTER_HELPER(idx, func) ebpf_register(vm, idx, #func, (void *)func)

void inject_helpers(ebpf_vm *vm)
{
	REGISTER_HELPER(1, bpf_map_lookup_elem);
	REGISTER_HELPER(2, bpf_map_update_elem);
	REGISTER_HELPER(3, bpf_map_delete_elem);
	REGISTER_HELPER(4, bpf_probe_read);
	REGISTER_HELPER(5, bpf_ktime_get_ns);
	REGISTER_HELPER(14, bpf_get_current_pid_tgid);
	REGISTER_HELPER(35, bpf_get_current_task);
	ebpf_register(vm, 113, "bpf_probe_read_kernel", (void *)bpf_probe_read);
	REGISTER_HELPER(131, bpf_ringbuf_reserve);
	REGISTER_HELPER(160, bpf_ktime_get_coarse_ns);
}
