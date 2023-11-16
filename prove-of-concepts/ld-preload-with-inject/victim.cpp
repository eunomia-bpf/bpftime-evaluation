#include <cstdio>
#include <unistd.h>
int main()
{
	while (true) {
		puts("abcd");
		usleep(1000 * 1000);
	}
	return 0;
}
