#include <iostream>

void example() {
    int&& rref = 42;
    int* ptr = &rref;

    std::cout << "rref: " << rref << std::endl;
    std::cout << "ptr: " << ptr << std::endl;
}

int main() {
    example();
    return 0;
}