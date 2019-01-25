char const *p;

void test() {
  char const str[] = "string";
  p = str; // warn
}
