#include "simple-jit/ebpf_vm.h"
#include <bpf/libbpf_legacy.h>
#include <cassert>
#include <cerrno>
#include <cinttypes>
#include "def.h"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iterator>
#include <linux/limits.h>
#include <ostream>
#include <bpf/libbpf.h>
#include <shared_mutex>
#include <string>
#include <map>
#include <sstream>
#include <unistd.h>
#include <unordered_map>
#include <utility>
#include <tuple>
#include <memory>
#include <mutex>
#include <vector>
#include <ebpf-core.h>
#include <string_view>
#include "vmlinux_mini.h"
#include "opensnoop.h"
#include <optional>
namespace
{
#include <syscall_list.h>
static inline std::pair<std::map<std::string, int>, std::map<int, std::string> >
generate_syscall_table()
{
	std::map<std::string, int> ret1;
	std::map<int, std::string> ret2;
	std::istringstream ss(table);
	while (ss) {
		std::string name;
		int id;
		ss >> name >> id;
		ret1[name] = id;
		ret2[id] = name;
	}
	return { ret1, ret2 };
}
} // namespace

static std::map<std::string, int> sys_name_to_id;
static std::map<int, std::string> sys_id_to_name;
static syscall_hooker_func_t orig_syscall;

struct EbpfVmContainer {
	// std::shared_mutex lock;
	ebpf_vm *vm;
	EbpfVmContainer()
	{
		vm = ebpf_create();
	}
	~EbpfVmContainer()
	{
		ebpf_destroy(vm);
	}
};

static std::vector<std::shared_ptr<EbpfVmContainer> > sys_enter_hooks[512];
static std::vector<std::shared_ptr<EbpfVmContainer> > sys_exit_hooks[512];

static const auto SYS_ENTER_PREFIX =
	std::string_view("tracepoint/syscalls/sys_enter_");
static const auto SYS_EXIT_PREFIX =
	std::string_view("tracepoint/syscalls/sys_exit_");

std::unordered_map<uint32_t, std::shared_ptr<args_t> > start_map;

extern "C" uint64_t bpf_get_current_pid_tgid(uint64_t, uint64_t, uint64_t,
					     uint64_t, uint64_t)
{
	return ((uint64_t)gettid() << 32) | getpid();
}

extern "C" uint64_t bpf_get_current_uid_gid(uint64_t, uint64_t, uint64_t,
					    uint64_t, uint64_t)
{
	return (((uint64_t)getgid()) << 32) | getuid();
}

extern "C" uint64_t bpf_map_lookup_elem(uint64_t map, uint64_t key, uint64_t,
					uint64_t, uint64_t)
{
	assert(map == 1);

	auto key_deref = *(uint32_t *)(uintptr_t)key;
	if (!start_map.contains(key_deref))
		return 0;
	return (uint64_t)(uintptr_t)start_map[key_deref].get();
}
extern "C" uint64_t bpf_map_update_elem(uint64_t map, uint64_t key,
					uint64_t value, uint64_t flag, uint64_t)
{
	assert(map == 1);
	auto key_deref = *(uint32_t *)(uintptr_t)key;
	auto val_deref = *(args_t *)(uintptr_t)value;
	start_map[key_deref] = std::make_shared<args_t>(std::move(val_deref));
	return 0;
}
extern "C" uint64_t bpf_map_delete_elem(uint64_t map, uint64_t key, uint64_t,
					uint64_t, uint64_t)
{
	assert(map == 1);
	auto key_deref = *(uint32_t *)(uintptr_t)key;
	auto itr = start_map.find(key_deref);
	if (itr != start_map.end())
		start_map.erase(itr);
	return 0;
}
extern "C" uint64_t bpf_perf_event_output(uint64_t, uint64_t map, uint64_t,
					  uint64_t data, uint64_t)
{
	assert(map == 2);
	auto event = (struct event *)(uintptr_t)data;
	// std::cout << "PERF EVENT ------- " << event->comm << " " <<
	// event->fname
	// 	  << " " << event->pid << " " << event->ret << std::endl;
	return 0;
}

extern "C" uint64_t bpf_get_current_comm(uint64_t buf, uint64_t size, uint64_t,
					 uint64_t, uint64_t)
{
	static std::optional<std::string> filename_buf;

	if (!filename_buf) {
		char strbuf[PATH_MAX];

		auto len = readlink("/proc/self/exe", strbuf,
				    std::size(strbuf) - 1);
		if (len == -1)
			return 1;
		strbuf[len] = 0;
		auto str = std::string_view(strbuf);
		auto last_slash = str.find_last_of('/');

		auto filename = std::string(str.substr(last_slash + 1));
		filename_buf = filename;
	}
	strncpy((char *)(uintptr_t)buf, filename_buf->c_str(), (size_t)size);
	return 0;
}

extern "C" uint64_t bpf_probe_read_user_str(uint64_t buf, uint64_t bufsz,
					    uint64_t ptr, uint64_t, uint64_t)
{
	strncpy((char *)(uintptr_t)buf, (const char *)(uintptr_t)ptr,
		(size_t)bufsz);
	return 0;
}

extern "C" uint64_t bpf_get_stack(uint64_t, uint64_t stack, uint64_t size,
				  uint64_t, uint64_t)
{
	memset((void *)(uintptr_t)stack, 0, (size_t)size);
	return 0;
}

static inline int
append_hook(const std::string_view &syscall_name, const std::string_view &name,
	    bpf_program *prog,
	    std::vector<std::shared_ptr<EbpfVmContainer> > *hook_arr)
{
	auto sys_id_itr = sys_name_to_id.find(std::string(syscall_name));
	if (sys_id_itr == sys_name_to_id.end()) {
		std::cerr << "Invalid syscall name " << syscall_name
			  << " for program " << name << std::endl;
		return -1;
	}
	auto insn_cnt = bpf_program__insn_cnt(prog);
	auto insns = bpf_program__insns(prog);
	auto vmc = std::make_shared<EbpfVmContainer>();

	ebpf_register(vmc->vm, 1, "bpf_map_lookup_elem",
		      (void *)bpf_map_lookup_elem);
	ebpf_register(vmc->vm, 2, "bpf_map_update_elem",
		      (void *)bpf_map_update_elem);
	ebpf_register(vmc->vm, 3, "bpf_map_delete_elem",
		      (void *)bpf_map_delete_elem);
	ebpf_register(vmc->vm, 14, "bpf_get_current_pid_tgid",
		      (void *)bpf_get_current_pid_tgid);
	ebpf_register(vmc->vm, 15, "bpf_get_current_uid_gid",
		      (void *)bpf_get_current_uid_gid);
	ebpf_register(vmc->vm, 16, "bpf_get_current_comm",
		      (void *)bpf_get_current_comm);
	ebpf_register(vmc->vm, 25, "bpf_perf_event_output",
		      (void *)bpf_perf_event_output);
	ebpf_register(vmc->vm, 67, "bpf_get_stack", (void *)bpf_get_stack);
	ebpf_register(vmc->vm, 114, "bpf_probe_read_user_str",
		      (void *)bpf_probe_read_user_str);

	char *errmsg = nullptr;
	if (int err = ebpf_load(vmc->vm, (const void *)insns,
				8 * (uint32_t)insn_cnt, &errmsg);
	    err < 0) {
		std::cerr << "Failed to load code into vm, err=" << err
			  << " message="
			  << (errmsg == nullptr ? "<unknown>" : errmsg)
			  << std::endl;
		if (errmsg)
			free(errmsg);
		return -1;
	}

	errmsg = nullptr;
	if (auto func = ebpf_compile(vmc->vm, &errmsg); !func) {
		std::cerr << "Failed to do compile on the ebpf function"
			  << std::endl;
		return -1;
	}
	ebpf_toggle_bounds_check(vmc->vm, false);
	hook_arr[sys_id_itr->second].push_back(vmc);
	return 0;
}
char ebpf_prog_path[512];

extern "C" int64_t hooker_func(int64_t sys_nr, int64_t arg1, int64_t arg2,
			       int64_t arg3, int64_t arg4, int64_t arg5,
			       int64_t arg6)
{
	if (!sys_enter_hooks[sys_nr].empty()) {
		trace_event_raw_sys_enter ctx;
		memset(&ctx, 0, sizeof(ctx));
		ctx.id = sys_nr;
		ctx.args[0] = arg1;
		ctx.args[1] = arg2;
		ctx.args[2] = arg3;
		ctx.args[3] = arg4;
		ctx.args[4] = arg5;
		ctx.args[5] = arg6;
		for (const auto &item : sys_enter_hooks[sys_nr]) {
			// std::shared_lock guard(item->lock);
			// Avoid polluting other ebpf programs..
			auto lctx = ctx;
			uint64_t ret;
			if (int err = ebpf_exec(item->vm, &lctx, sizeof(lctx),
						&ret);
			    err < 0) {
				std::cerr
					<< "Failed to execute callback hooker at entering "
					<< err << " for sysnr " << sys_nr
					<< std::endl;
			}
		}
	}
	// std::cout << "Syscall " << sys_nr << ": " << arg1 << " " << arg2 << "
	// "
	// 	  << arg3 << " " << arg4 << " " << arg5 << " " << arg6
	// 	  << std::endl;
	int64_t ret = orig_syscall(sys_nr, arg1, arg2, arg3, arg4, arg5, arg6);
	if (!sys_exit_hooks[sys_nr].empty()) {
		trace_event_raw_sys_exit ctx;
		memset(&ctx, 0, sizeof(ctx));
		ctx.id = sys_nr;
		ctx.ret = ret;
		for (const auto &item : sys_exit_hooks[sys_nr]) {
			// std::shared_lock guard(item->lock);
			// Avoid polluting other ebpf programs..
			auto lctx = ctx;
			uint64_t ret;
			if (int err = ebpf_exec(item->vm, &lctx, sizeof(lctx),
						&ret);
			    err < 0) {
				std::cerr
					<< "Failed to execute callback hooker at exiting "
					<< err << " for sys_nr " << sys_nr
					<< std::endl;
			}
		}
	}
	return ret;
}
extern "C" int setup_hooker(syscall_hooker_func_t *hooker)
{
	orig_syscall = *hooker;
	*hooker = hooker_func;
	LIBBPF_OPTS(bpf_object_open_opts, open_opts);
	auto obj = bpf_object__open_file(ebpf_prog_path, &open_opts);
	if (!obj) {
		std::cerr << "Failed to open bpf object: " << strerror(errno)
			  << std::endl;
		return -1;
	}
	std::tie(sys_name_to_id, sys_id_to_name) = generate_syscall_table();
	std::cout << "Loaded " << sys_name_to_id.size() << " syscalls"
		  << std::endl;
	bpf_program *prog;
	bpf_object__for_each_program(prog, obj)
	{
		auto sec_name =
			std::string_view(bpf_program__section_name(prog));
		auto name = bpf_program__name(prog);

		if (sec_name.starts_with(SYS_ENTER_PREFIX)) {
			if (int err = append_hook(
				    sec_name.substr(SYS_ENTER_PREFIX.size()),
				    name, prog, sys_enter_hooks);
			    err < 0) {
				return err;
			}
			std::cout << "Loaded sysenter hook for section "
				  << sec_name << " and prog name " << name
				  << std::endl;
		} else if (sec_name.starts_with(SYS_EXIT_PREFIX)) {
			if (int err = append_hook(
				    sec_name.substr(SYS_EXIT_PREFIX.size()),
				    name, prog, sys_exit_hooks);
			    err < 0) {
				return err;
			}

			std::cout << "Loaded sysexit hook for section "
				  << sec_name << " and prog name " << name
				  << std::endl;
		} else {
			std::cout << "Ignoring program `" << name
				  << "`, section " << sec_name << std::endl;
		}
	}
	return 0;
}

// extern "C" int64_t hooker(int64_t sys_nr, int64_t arg1, int64_t arg2,
// 			  int64_t arg3, int64_t arg4, int64_t arg5,
// 			  int64_t arg6)
// {
// 	return 0;
// }
// int main()
// {
// 	strcpy(ebpf_prog_path, "../opensnoop.bpf.o");
// 	syscall_hooker_func_t hooker = ::hooker;
// 	setup_hooker(&hooker);
// 	while (true) {
// 		hooker(257, 1, (int64_t)(uintptr_t) "aaabbb", 3, 4, 5, 6);
// 	}
// 	return 0;
// }
