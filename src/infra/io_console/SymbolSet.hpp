#pragma once

#include <string>

namespace Infrastructure::IOConsole {

enum class SymbolSetId {
    Ascii,
    UnicodeSimple,
};

struct SymbolSet {
    // Map tiles
    std::string wall;
    std::string floor;
    std::string unknown_tile;

    // Entities
    std::string player;
    std::string enemy;
    std::string item;

    // HUD
    std::string hp_label;
};

[[nodiscard]] const SymbolSet &get_symbol_set(SymbolSetId id) noexcept;

} // namespace Infrastructure::IOConsole
