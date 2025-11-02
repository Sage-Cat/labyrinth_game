#pragma once
#include <string_view>

namespace Infrastructure::Log
{
    struct Logger
    {
        void info(std::string_view) {}
        void error(std::string_view) {}
        void debug(std::string_view) {}
    };
}
