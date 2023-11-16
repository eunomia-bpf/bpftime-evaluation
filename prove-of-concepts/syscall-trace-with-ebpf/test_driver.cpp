#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <unistd.h>
#include <string>
#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)

int main()
{
	// Run the victim
	if (int pid = fork(); pid > 0) {
		// Run the injector
		// Usage: ./injector <PID to inject> <ebpf program> [agent path
		// (default to agent.so)] [hooker path (default to ./hooker.so)]
		std::string cmd_line =
			"\"" + std::string(STRINGIFY(INJECTOR_PATH)) + "\"";
		cmd_line += " \"" + std::to_string(pid) + "\"";
		cmd_line += " \"" + std::string(STRINGIFY(OPENSNOOP_BPF_PATH)) +
			    "\"";
		cmd_line += " \"" + std::string(STRINGIFY(AGENT_PATH)) + "\"";
		cmd_line += " \"" + std::string(STRINGIFY(HOOKER_PATH)) + "\"";
		std::cout << "Run injector: " << cmd_line << std::endl;
		if (int ret = system(cmd_line.c_str()); ret != 0) {
			std::cerr << "Injection failed: " << ret << std::endl;
			kill(pid, SIGTERM);
			return 1;
		}
		// if (int err = kill(pid, SIGTERM); err == -1) {
		// 	std::cout << "Kill failed: " << strerror(errno)
		// 		  << std::endl;
		// }
		// Wait for 10s and see results..
		std::cout
			<< "Injection successfully. Will wait for 10s and see results.."
			<< std::endl;
		usleep(10 * 1000 * 1000);
		kill(pid, SIGINT);
		std::cout << "Exiting.." << std::endl;
	} else {
		execl(STRINGIFY(VICTIM_PATH), STRINGIFY(VICTIM_PATH), nullptr);
	}
	return 0;
}
