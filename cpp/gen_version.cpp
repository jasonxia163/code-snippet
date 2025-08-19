#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>

uint64_t GetVersionNumber(const std::string& version) {
    // 1.0.554-92cc421
    std::istringstream iss(version);
    uint64_t major, minor, patch;
    char dot;
    iss >> major >> dot >> minor >> dot >> patch;
    std::cout << "major: " << major
              << ", minor: " << minor
              << ", patch: " << patch << std::endl;
    // reserved the last 8bit for future use
    return (major << 48) | (minor << 32) | (patch << 8);
}

int main() {
    std::string version = "1.0.554-92cc421";
    uint64_t version_number = GetVersionNumber(version);
    std::cout << std::hex << std::showbase << version_number << std::endl;
    return 0;
}