unsigned int A;
unsigned int B;
void test(unsigned int x, unsigned int y)
{
  /*@ assert Value: division_by_zero: (unsigned int)(x*y) ≢ 0; */
  A = 100u / (x * y);
  /*@ assert Value: division_by_zero: x ≢ 0; */
  B = 333u % x;
  return;
}
