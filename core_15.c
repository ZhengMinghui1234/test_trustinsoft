int G;
void main(int x, int y)
{
  if (y < 0) 
    /*@ assert Value: division_overflow: x ≢ -2147483648 ∨ y ≢ -1; */
    G = x % y;
  return;
}
