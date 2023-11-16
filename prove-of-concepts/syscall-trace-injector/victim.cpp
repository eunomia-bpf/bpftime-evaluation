#include <fstream>
#include <iostream>
#include <ostream>
#include <unistd.h>
int main()
{
	// Sleep 3s for injecting..
	usleep(3 * 1000 * 1000);
	std::ofstream ofs("out.txt");
	std::cout << "Writing to file out.txt" << std::endl;
	ofs << "Hello from " << getpid() << std::endl;
	std::cout << "Sleep for 5s" << std::endl;
	for (int i = 1; i <= 5; i++) {
		ofs << ".";
		ofs.flush();
		std::cout << ".";
		std::cout.flush();
		usleep(1000 * 1000);
	}
	std::cout << "Exiting.." << std::endl;
	ofs << "By from " << getpid() << std::endl;
	return 0;
}
