int test_url(const char *str) {
  while (*str) {
    if (*str == 'X')
      return 1;
    str++;
  }
  return 0;
}
