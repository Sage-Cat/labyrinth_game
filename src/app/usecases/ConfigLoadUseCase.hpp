#pragma once
namespace Domain::Rules
{
    struct GameRules;
}

namespace Application::Usecases
{
    struct ConfigLoadUseCase
    {
        static Domain::Rules::GameRules load_default();
    };
}
