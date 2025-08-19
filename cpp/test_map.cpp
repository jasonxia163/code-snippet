#include <iostream>
#include <map>

int main() {
  std::map<int, std::string> map;
  map[1] = "1";
  map[2] = "2";
  map[3] = "3";
  for (auto& [key, value] : map) {
    std::cout << key << " " << value << std::endl;
  }
  return 0;
}