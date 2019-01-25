void f(int *p);

void testUseMiddleArgAfterDelete(int *p) {
  delete p;
  f(p); // warn: use after free
}
