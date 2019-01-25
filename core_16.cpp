class A {
public:
  A() {
    f(); // warn
  }
  virtual void f();
};
