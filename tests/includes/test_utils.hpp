#pragma once
#include <iostream>
#include <string>

namespace TestUtils {

inline int fail_count = 0;

inline bool expect(const bool condition, const std::string &name)
{
    if (condition) {
        std::cout << "[PASS]" << " " << name << "\n";
        return true;
    } else {
        std::cout << "[FAIL]" << " " << name << "\n";
        fail_count++;
        return false;
    }
}

inline void reset_fail_count() { fail_count = 0; }

} // namespace TestUtils