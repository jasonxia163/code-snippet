// Copyright 2024 jason.xia

#include <iostream>
#include <memory>

class Test {
 public:
  explicit Test(int value) : value_(value) {
    std::cout << "Test constructor" << std::endl;
  }

  ~Test() { std::cout << "Test destructor" << std::endl; }

 private:
  int value_;
};

void test_func(const std::shared_ptr<Test>& test) {
  if (test == nullptr) {
    std::cout << "test is nullptr" << std::endl;
  } else {
    std::cout << "test is not nullptr" << std::endl;
  }
}

int main() {
  // nullptr 是右值，不能取地址
  // &nullptr;  // 错误：不能取 nullptr 的地址

  // nullptr 不能作为左值
  // nullptr = std::shared_ptr<Test>();  // 错误：不能给 nullptr 赋值

  // 正确：常量引用可以绑定到右值 nullptr
  const std::shared_ptr<Test>& const_test = nullptr;

  // 正确：使用指针而不是引用
  std::shared_ptr<Test>* ptr_test = nullptr;

  // 正确：直接使用 shared_ptr 对象
  std::shared_ptr<Test> test = nullptr;

  test_func(nullptr);
}