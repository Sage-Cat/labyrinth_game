#include "domain/services/MapGen.hpp"
#include "domain/entities/Map.hpp"
#include "domain/rules/GameRules.hpp"

#include "MapGenHelpers.hpp"

namespace Domain::Services {
void MapGen::generate(const Domain::Rules::GameRules &, Domain::Core::IRng &,
                      Domain::Entities::Map &)
{
    // TODO: generate map based on rules and size
}
} // namespace Domain::Services
