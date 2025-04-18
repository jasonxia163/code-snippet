// Copyright 2024 jason.xia

#include <iostream>

class TestStatic {
 public:
  explicit TestStatic(uint32_t id) : id_(id) {}

  void show_info() {
    static uint32_t count = 0;
    count++;
    std::cout << "id: " << id_ << ", count: " << count << std::endl;
  }

 private:
  uint32_t id_;
};

int main() {
  TestStatic test1(1);
  test1.show_info();
  test1.show_info();
  TestStatic test2(2);
  test2.show_info();
  test2.show_info();
}