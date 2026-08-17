#pragma once

#include <cctype>
#include <optional>
#include <string>
#include <type_traits>

namespace Infrastructure::IOConsole {

template <typename T>
concept HasMemberUtf8 = requires(const T &t) {
    { t.utf8 } -> std::convertible_to<std::string>;
};

template <typename T>
concept HasMemberSymbol = requires(const T &t) {
    { t.symbol } -> std::convertible_to<std::string>;
};

template <typename T>
concept HasMemberCh = requires(const T &t) { t.ch; };

namespace detail {

[[nodiscard]] inline bool is_placeholder_string(const std::string &s) noexcept
{
    if (s.empty()) {
        return true;
    }

    // Treat a single "?" as placeholder (common default)
    if (s.size() == 1 && s[0] == '?') {
        return true;
    }

    // Treat “all whitespace” as placeholder
    for (unsigned char c : s) {
        if (!std::isspace(c) && c != '\0') {
            return false;
        }
    }
    return true;
}

[[nodiscard]] inline bool is_valid_char_glyph(char c) noexcept
{
    // Reject common “empty/default” values
    if (c == '\0' || c == '?' || std::isspace(static_cast<unsigned char>(c))) {
        return false;
    }
    
    // Require visible single-cell ASCII for now
    // TODO: wider codepoints
    return std::isgraph(static_cast<unsigned char>(c)) != 0;
}

} // namespace detail

// A “valid” glyph must be explicitly meaningful
template <typename GlyphT>
[[nodiscard]] inline std::optional<std::string> glyph_to_utf8(const GlyphT &g)
{
    if constexpr (HasMemberUtf8<GlyphT>) {
        const std::string s = g.utf8;
        if (!detail::is_placeholder_string(s)) {
            return s;
        }
    }

    if constexpr (HasMemberSymbol<GlyphT>) {
        const std::string s = g.symbol;
        if (!detail::is_placeholder_string(s)) {
            return s;
        }
    }

    if constexpr (HasMemberCh<GlyphT>) {
        using ChT = std::remove_cvref_t<decltype(g.ch)>;
        if constexpr (std::is_same_v<ChT, char>) {
            if (detail::is_valid_char_glyph(g.ch)) {
                return std::string{g.ch};
            }
        }
    }

    return std::nullopt;
}

template <typename StatsT>
concept HasHpAndHpMax = requires(const StatsT &s) {
    { s.hp } -> std::convertible_to<int>;
    { s.hp_max } -> std::convertible_to<int>;
};

template <typename StatsT>
concept HasHpOnly = requires(const StatsT &s) {
    { s.hp } -> std::convertible_to<int>;
};

} // namespace Infrastructure::IOConsole
