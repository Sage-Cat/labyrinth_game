#include "infra/io_console/SymbolSet.hpp"

namespace Infrastructure::IOConsole {

const SymbolSet &get_symbol_set(SymbolSetId id) noexcept
{
    static const SymbolSet ascii{
        .wall         = "#",
        .floor        = " ",
        .unknown_tile = "?",
        .player       = "@",
        .enemy        = "E",
        .item         = "*",
        .hp_label     = "HP",
    };

    // More pretty. Let's keep it as an option
    static const SymbolSet unicode_simple{
        .wall         = "█",
        .floor        = "⠀",
        .unknown_tile = "?",
        .player       = "☻",
        .enemy        = "☠",
        .item         = "♦",
        .hp_label     = "♥",
    };

    switch (id) {
    case SymbolSetId::UnicodeSimple:
        return unicode_simple;
    case SymbolSetId::Ascii:
    default:
        return ascii;
    }
}

} // namespace Infrastructure::IOConsole
