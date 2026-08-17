#pragma once
#include "app/loop/IRenderer.hpp"
#include "infra/io_console/SymbolSet.hpp"

namespace Infrastructure::IOConsole
{
    class ConsoleRenderer final : public Application::Loop::IRenderer
    {
    public:
        explicit ConsoleRenderer(SymbolSetId symbols_id = SymbolSetId::Ascii)
            : symbols_id_(symbols_id)
        {
        }

        void set_symbol_set(SymbolSetId symbols_id) { symbols_id_ = symbols_id; }
        SymbolSetId symbol_set() const { return symbols_id_; }

        void draw(const Domain::Core::GameState &) override;

    private:
        SymbolSetId symbols_id_{SymbolSetId::Ascii};
    };
}
