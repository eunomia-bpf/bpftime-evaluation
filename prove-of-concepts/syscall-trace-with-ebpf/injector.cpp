#include <cstring>
#include <frida-core.h>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <ostream>
#include <memory>
#include <functional>
#include "def.h"
#include <string>
int main(int argc, char **argv)
{
	if (argc < 3) {
		std::cerr
			<< "Usage: " << argv[0]
			<< " <PID to inject> <ebpf program> [agent path (default to agent.so)] [hooker path (default to ./hooker.so)]"
			<< std::endl;
		return 1;
	}
	int pid = atoi(argv[1]);
	if (pid == 1) {
		std::cerr << "Invalid pid (digits error?)" << std::endl;
		return 1;
	}
	std::string arg_to_agent;
	arg_to_agent += argv[2]; // ebpf program
	arg_to_agent += ":";
	arg_to_agent += argc >= 5 ? argv[4] : "./hooker.so"; // hooker path
	struct Dropper {
		Dropper()
		{
			frida_init();
		}
		~Dropper()
		{
			frida_deinit();
		}
	} _dropper;
	auto drop = [](FridaInjector *inj) {
		GError *err = nullptr;
		frida_injector_close_sync(inj, nullptr, &err);
		if (err) {
			std::cerr << "Failed to destroy injector: "
				  << err->message << std::endl;
			g_clear_error(&err);
			exit(1);
		}
	};
	std::unique_ptr<FridaInjector, decltype(drop)> injector(
		frida_injector_new(), drop);
	GError *err = nullptr;
	frida_injector_inject_library_file_sync(
		injector.get(), pid, argc >= 4 ? argv[3] : "agent.so",
		"agent_main", arg_to_agent.c_str(), nullptr, &err);
	if (err) {
		std::cerr << "Failed to attach: " << err->message;
		g_clear_error(&err);
		return 1;
	}
	return 0;
}
