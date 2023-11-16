#include <cassert>
#include <cstring>
#include <ios>
#include <iterator>
#include <signal.h>
#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <ostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>
#include <chrono>
static uint64_t total_time_usage = 0;
static uint64_t total_count = 0;
extern "C" void sig_handler(int sig)
{
	if (sig == SIGTERM)
		return;
	std::cout << "open/openat performed " << total_count
		  << " times, total time usage " << total_time_usage
		  << " ns, average " << std::fixed << std::setprecision(6)
		  << (double)total_time_usage / total_count << " ns"
		  << std::endl;
	exit(1);
}

int main(int argc, char **argv)
{
	signal(SIGTERM, sig_handler);
	signal(SIGINT, sig_handler);

	srand(time(nullptr));
	char dir_name[256] = "/tmp/opensnoop.test.XXXXXX";
	auto ret = mkdtemp(dir_name);
	assert(ret == dir_name);
	if (argc >= 2 && strcmp(argv[1], "wait-for-sig") == 0) {
		std::cout << "Waiting for signal" << std::endl;
		pause();
		std::cout << "signal caught" << std::endl;
	}
	while (true) {
		auto local_path_name = std::string(dir_name) +
				       std::string("/") +
				       std::to_string(rand());
		auto start_time = std::chrono::high_resolution_clock::now();
		int fd = open(local_path_name.c_str(), O_CREAT | O_WRONLY);
		auto end_time = std::chrono::high_resolution_clock::now();

		auto elapsed =
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				end_time - start_time)
				.count();
		total_count += 1;
		total_time_usage += elapsed;

		auto write_content = std::to_string(time(nullptr));
		write(fd, write_content.c_str(), write_content.size());
		close(fd);
		std::cout << "Written " << write_content << " to "
			  << local_path_name << std::endl;
		usleep(500 * 1000);
	}
	return 0;
}
