#include <iostream>
#include <type_traits>

class ClassA {
 public:
  explicit ClassA(int x) : x_(x) {}
  int getX() const { return x_; }

 private:
  int x_;
};

class ClassB {
 public:
  explicit ClassB(int y) : y_(y) {}
  int getY() const { return y_.getX(); }

 private:
  ClassA y_;
};

template <typename T>
void RunGet(T obj) {
  if constexpr (std::is_same_v<T, ClassA>) {
    std::cout << obj.getX() << std::endl;
  } else if constexpr (std::is_same_v<T, ClassB>) {
    std::cout << obj.getY() << std::endl;
  }
}

int main() {
  auto a = ClassA(1);
  auto b = ClassB(2);
  RunGet(a);
  RunGet(b);
  return 0;
}