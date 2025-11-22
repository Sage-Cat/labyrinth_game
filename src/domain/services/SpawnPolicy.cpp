#include "domain/services/SpawnPolicy.hpp"
#include "domain/entities/Map.hpp"
#include "domain/entities/actors/Enemy.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/entities/items/HealthPotion.hpp"
#include "domain/entities/items/Item.hpp"
#include "domain/entities/items/Key.hpp"
#include "domain/rules/GameRules.hpp"

namespace Domain::Services {
void SpawnPolicy::place_player(const Domain::Rules::GameRules &, Domain::Core::IRng &,
                               Domain::Entities::Map &, Domain::Entities::Player &)
{
    // TODO: spawn player in random place
}

void SpawnPolicy::place_enemies(const Domain::Rules::GameRules &, Domain::Core::IRng &,
                                Domain::Entities::Map &, std::vector<Domain::Entities::Enemy> &)
{
    // TODO: spawn a number of enemies
}

void SpawnPolicy::place_items(const Domain::Rules::GameRules &, Domain::Core::IRng &,
                              Domain::Entities::Map &,
                              std::vector<std::unique_ptr<Domain::Entities::Item>> &)
{
    // TODO: spawn a number of items (at least 1 key)
}
} // namespace Domain::Services
