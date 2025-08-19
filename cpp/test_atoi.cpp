#include <iostream>
#include <string>
#include <algorithm>

int main() {
  std::string max_mem = "2202009600\n";

  #if 0
  max_mem.erase(0, max_mem.find_first_not_of(" \t\n\r\f\v"));
  size_t last_non_whitespace = max_mem.find_last_not_of(" \t\n\r\f\v");
  if (last_non_whitespace != std::string::npos) {
    max_mem.erase(last_non_whitespace + 1);
  } else {
    max_mem.clear();
  }
  #endif

  std::cout << "after process: '" << max_mem << "'" << std::endl;

  uint64_t max_memory = std::stoull(max_mem);
  std::cout << "max int: " << INT_MAX << std::endl;
  std::cout << "max uint64_t: " << UINT64_MAX << std::endl;
  std::cout << max_memory << std::endl;
  return 0;
}