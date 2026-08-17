
#pragma once
#include <string_view>
namespace Domain::Core {
constexpr std::string_view version() noexcept { return std::string_view{"0.1.0"}; }
} // namespace Domain::Core
