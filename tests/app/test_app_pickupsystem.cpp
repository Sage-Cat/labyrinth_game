#include "app/systems/PickupSystem.hpp"

#include "domain/core/GameState.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/entities/items/Coin.hpp"
#include "domain/entities/items/HealthPotion.hpp"
#include "domain/entities/items/Key.hpp"
#include "domain/entities/items/Sword.hpp"

#include <cassert>
#include <memory>

using namespace Domain::Entities;
using namespace Domain::Core;
using namespace Application::Systems;

struct TestState {
    GameState state;
    Player *player;
};

static TestState make_state()
{
    TestState t;

    auto player = std::make_unique<Player>();
    player->pos = {1, 1};

    t.player = player.get();
    t.state.actors.push_back(std::move(player));

    return t;
}

class UnknownItem final : public Item {};

void test_ignore_unknown_item()
{

    auto t = make_state();

    t.player->stats.hp = 5;

    auto unknown_item = std::make_unique<UnknownItem>();
    unknown_item->pos = {1, 1};
    t.state.items.push_back(std::move(unknown_item));

    PickupSystem system;
    system.process(t.state);

    assert(!t.player->has_key);
    assert(t.player->stats.hp == 5);
    assert(t.state.items.size() == 1);
}

void test_pickup_key()
{
    auto t = make_state();

    auto key = std::make_unique<Key>();
    key->pos = {1, 1};

    t.state.items.push_back(std::move(key));

    PickupSystem system;
    system.process(t.state);

    assert(t.player->has_key);
    assert(t.state.items.empty());
}

void test_pickup_health_potion()
{
    auto t = make_state();

    t.player->stats.hp     = 5;
    t.player->stats.max_hp = 10;

    auto potion          = std::make_unique<HealthPotion>();
    potion->pos          = {1, 1};
    potion->healingValue = 5;
    t.state.items.push_back(std::move(potion));

    PickupSystem system;
    system.process(t.state);

    assert(t.player->stats.hp > 5);
    assert(t.player->stats.hp <= t.player->stats.max_hp);
    assert(t.state.items.empty());
}

void test_pickup_doesnt_exceed_max_hp()
{
    auto t = make_state();

    t.player->stats.hp     = 9;
    t.player->stats.max_hp = 10;

    auto potion          = std::make_unique<HealthPotion>();
    potion->pos          = {1, 1};
    potion->healingValue = 5;
    t.state.items.push_back(std::move(potion));

    PickupSystem system;
    system.process(t.state);

    assert(t.player->stats.hp == t.player->stats.max_hp);
    assert(t.state.items.empty());
}

void test_pickup_on_max_hp()
{
    auto t = make_state();

    t.player->stats.hp     = 10;
    t.player->stats.max_hp = 10;

    auto potion          = std::make_unique<HealthPotion>();
    potion->pos          = {1, 1};
    potion->healingValue = 5;
    t.state.items.push_back(std::move(potion));

    PickupSystem system;
    system.process(t.state);

    assert(t.player->stats.hp == t.player->stats.max_hp);
    assert(t.state.items.empty());
}

void test_pickup_multiple_items()
{
    auto t = make_state();

    t.player->stats.hp     = 5;
    t.player->stats.max_hp = 10;

    auto key = std::make_unique<Key>();
    key->pos = {1, 1};
    t.state.items.push_back(std::move(key));

    auto potion          = std::make_unique<HealthPotion>();
    potion->pos          = {1, 1};
    potion->healingValue = 5;
    t.state.items.push_back(std::move(potion));

    PickupSystem system;
    system.process(t.state);

    assert(t.player->has_key);
    assert(t.player->stats.hp > 5);
    assert(t.state.items.empty());
}

void test_item_on_other_tile()
{
    auto t = make_state();

    auto key = std::make_unique<Key>();
    key->pos = {5, 5};

    t.state.items.push_back(std::move(key));

    PickupSystem system;
    system.process(t.state);

    assert(!t.player->has_key);
    assert(t.state.items.size() == 1);
}

void test_pickup_sword()
{
    auto t = make_state();

    t.player->stats.atk = 5;

    auto sword          = std::make_unique<Sword>();
    sword->pos          = {1, 1};
    sword->attack_bonus = 5;
    t.state.items.push_back(std::move(sword));

    PickupSystem system;
    system.process(t.state);

    assert(t.player->stats.atk == 10);
    assert(t.state.items.empty());
}

void test_pickup_coin()
{
    auto t = make_state();

    auto coin   = std::make_unique<Coin>();
    coin->pos   = {1, 1};
    coin->value = 1;
    t.state.items.push_back(std::move(coin));

    PickupSystem system;
    system.process(t.state);

    assert(t.state.items.empty());
    assert(t.state.score == 1);
}

int main()
{
    test_pickup_key();
    test_pickup_health_potion();
    test_pickup_doesnt_exceed_max_hp();
    test_pickup_on_max_hp();
    test_pickup_multiple_items();
    test_item_on_other_tile();
    test_ignore_unknown_item();
    test_pickup_sword();
    test_pickup_coin();

    return 0;
}