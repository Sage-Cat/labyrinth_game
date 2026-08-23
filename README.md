# Labyrinth

Labyrinth is a C++20 console roguelike. The MVP generates a seeded dungeon, supports player movement, enemy pursuit and combat, and collectible keys, potions, swords, and coins.

## Prerequisites

- CMake 3.20 or newer
- A C++20 compiler (GCC, Clang, or MSVC)

## Build and run

```sh
cmake --preset release
cmake --build --preset release
./build/release/bin/labyrinth
```

Use `--seed=<number>` for a reproducible dungeon and `--symbols=unicode` for Unicode glyphs. ASCII is the default.

## Menu and session flow

On startup the game presents a small console menu (new game / quit, depending on build). A new game builds a seeded dungeon map, places the player, enemies, and pickups, then enters the turn-based game loop.

## Controls

- `W`, `A`, `S`, `D` or arrow keys: move one tile
- `.`: wait one turn
- `Q`: quit

When input is redirected, commands may also be written as `up`, `down`, `left`, `right`, `quit`, or `exit`, one per line.

## Items

Pickups found on the dungeon floor include:

- **Keys** — required progress items for locked content where applicable
- **Potions** — restore hit points when collected
- **Swords** — improve combat effectiveness
- **Coins** — score / collectible currency

Exact combat and heal numbers are defined in the domain rules and applied by `PickupSystem` / `CombatSystem`.

## Combat and enemies

Enemies act after the player each turn (`EnemyAISystem`): they path toward the player when possible and engage in melee via `CombatSystem`. Bumping an adjacent enemy attacks; dying enemies are removed from the map.

## Win and lose

`WinLoseSystem` evaluates end conditions each turn:

- **Lose** when the player's hit points reach zero
- **Win** when the configured victory rule is satisfied (for example collecting the required keys / reaching the exit — see domain win policy)

The loop stops when a terminal win or lose state is reached.

## Save path

Save/load use-cases exist under `src/app/persistence` and `src/app/usecases`. File-backed save/load is still incomplete in the MVP (load currently reports failure / stub behavior); do not rely on save files yet. Prefer a seeded new game (`--seed=`) for reproducible sessions.

## Tests

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

The debug preset enables AddressSanitizer and UndefinedBehaviorSanitizer on GCC and Clang. A non-sanitized `release` test preset is also available.

## Architecture diagrams

PlantUML sources and rendered PNGs live under [`docs/diagrams/`](docs/diagrams/). They describe the component layers, domain model, game-loop sequence, enemy AI activity, and general state machine. Prefer the `.puml` sources if a PNG is stale.

## License

Released under the [MIT License](LICENSE).
