#include <sys/sdt.h>

void myfunction(char *str, int num) {
  DTRACE_PROBE2(myprovider, myprobe, str, num);
  // Rest of function...
}

int main() {
  myfunction("Hello, world!", 42);
  return 0;
}