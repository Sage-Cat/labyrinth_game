#include <cstdlib>
#include <iostream>

#include "domain/core/GameState.hpp"
#include "domain/entities/actors/Player.hpp"
#include "domain/services/WinLosePolicy.hpp"

[[nodiscard]] bool test_hp_less_than_zero() {
  Domain::Core::GameState state{};
  auto player = std::make_unique<Domain::Entities::Player>();
  player->stats.hp = -5;
  state.actors.push_back(std::move(player));
  Domain::Services::WinLosePolicy::evaluate(state);

  return state.defeat;
}

[[nodiscard]] bool test_hp_equel_zero() {
  Domain::Core::GameState state{};
  auto player = std::make_unique<Domain::Entities::Player>();
  player->stats.hp = 0;
  state.actors.push_back(std::move(player));

  Domain::Services::WinLosePolicy::evaluate(state);

  return state.defeat;
}

[[nodiscard]] bool test_hp_greater_than_zero() {
  Domain::Core::GameState state{};
  auto player = std::make_unique<Domain::Entities::Player>();
  player->stats.hp = 5;
  state.actors.push_back(std::move(player));

  Domain::Services::WinLosePolicy::evaluate(state);

  return !state.defeat;
}
int main() {
  const bool all_passed = test_hp_less_than_zero() && test_hp_equel_zero() &&
                          test_hp_greater_than_zero();

  return all_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
