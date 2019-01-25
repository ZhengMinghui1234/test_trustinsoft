// C
void test(int *p) {
  if (p)
    return;

  int x = p[0]; // warn
}
