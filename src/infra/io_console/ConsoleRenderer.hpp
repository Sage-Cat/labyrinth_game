#pragma once

#include "app/loop/IRenderer.hpp"
#include "infra/io_console/SymbolSet.hpp"

namespace Infrastructure::IOConsole {

class ConsoleRenderer final : public Application::Loop::IRenderer {
public:
    ConsoleRenderer() = default;
    explicit ConsoleRenderer(SymbolSetId id) : symbols_id_(id) {}

    void set_symbol_set(SymbolSetId id) noexcept { symbols_id_ = id; }

    void draw(const Domain::Core::GameState &) override;

private:
    SymbolSetId symbols_id_{SymbolSetId::Ascii};
};

} // namespace Infrastructure::IOConsole
