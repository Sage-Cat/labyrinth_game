#pragma once
#include <string>
namespace Domain::Core
{
    struct GameState;
}
namespace Application::Persistence
{
    struct ISaveGameRepo
    {
        virtual ~ISaveGameRepo() = default;
        virtual bool save(const Domain::Core::GameState &, const std::string &path) = 0;
        virtual bool load(Domain::Core::GameState &, const std::string &path) = 0;
    };
}
