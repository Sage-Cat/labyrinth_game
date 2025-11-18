# Development Plan for "labyrinth" Console Game

## Overview

A console-based rogue-like game called **"labyrinth"**, written in modern C++ (C++20), using STL, CMake for build system, and Git for version control. The game features a randomly generated 2D labyrinth, collectible items, simple enemies, and save/load functionality.

---

## 1. Project Setup

### 1.1 Tools

* **Language**: Modern C++20
* **Build System**: CMake
* **Version Control**: Git
* **IDE/Text Editor**: VSCode
* **Testing**: ctest
* **CI/CD (optional)**: Jenkins

---

## 2. Requirements Analysis

### 2.1 Functional Requirements

* Random labyrinth map (NxM grid, max_N=255) - '#' — wall, ' ' — floor, '>' - exit, '@' — character, 'E' — enemy, '*' — item (could be either key or hp potion).
* Character movement (WASD) or (arrow keys)
* Item collection. Autousage. No inventory mechanic, just overview in HUD
* Enemies with simple AI (one step towards the character after the event 'see player' (no walls between you and him))
* Combat mechanics (hp-based - one move, one hit for some number of hp)
* Victory condition (exit `>` appears/is opened after player get's a key)
* Defeat condition (HP ≤ 0)
* Menu (Start New Game, Load, Exit)
* Save/load system

### 2.2 Non-functional Requirements

* Cross-platform (Windows/Linux) - C++20 and STD, no third party
* Fast compilation and modular code - plantuml architecture design
* Clear documentation (README + comments) - development plan, readme, licence, user manual
* Small binary footprint - one executable, no dynamic libs
* Testable components - partly TDD development, basic unit tests

---

## 3. System Design

This is a concise architecture for a console roguelike in modern C++ (C++20), STL-only, CMake build, Git for VCS. It lists entities/modules and their roles.

---

## Architectural Layers
- **Domain** — pure gameplay logic and data (no I/O).
- **Application** — orchestrates domain via systems and use-cases.
- **Infrastructure** — console I/O, persistence, config, RNG, clock, logging.
- `main` - wiring and dependency construction (in the root of src/).

---

## Domain Model (Entities & Value Types)
- **EntityId**: `uint32_t` identifier.
- **Position**: `{uint16_t x, uint16_t y}`.
- **Direction**: enum `{Up, Down, Left, Right}`.
- **Stats**: `{hp, max_hp, atk, def}`.
- **Tile**: `{type: Wall|Floor, blocks_movement: bool, blocks_sight: bool}`.
- **Map**: `Grid<Tile>` with bounds/passability/LOS helpers.
- **ItemKind**: enum `{Key, HealthPotion}`. HealthPotion used auto when `max_hp - hp >= potion_heal`
- **Item**: `{kind: ItemKind, value: int}`.
- **Player**: `Position, Stats, InventoryFlags{has_key}`.
- **Enemy**: `Position, Stats, AIChase`. AIChase - start chase when Player in LOS and does not end until fight. Movement - nearest path search.
- **Glyph**: `{char ch}` (render hint; still domain-safe). It's just a smallest texture block.
- **Rng**: domain-facing PRNG facade (seed from app). It's some randomness generator for textures.
- **GameRules**: constants (map size, drop rates, damage policy).
- **GameState** (aggregate) - some "per-turn" state that is managed in a main game loop. Example:
  - `Map`
  - entity registry (players, enemies, items)
  - turn state, victory/defeat flags, score

---

## Domain Services (Pure Policies)
- **LOS**: Bresenham-based line-of-sight. Decide if two grid cells can “see” each other by tracing the discrete line between their centers and checking if any tile on that line blocks sight.
- **Pathfinding**: minimal BFS/greedy single-step toward target.
- **SpawnPolicy**: place player, enemies, items.
- **WinLosePolicy**: evaluate victory/defeat. Two possibilities: exit or kill all enemies.
- **MapGen**: rooms-and-corridors or drunkard walk → `Map`.

---

## Application Layer (Use-Cases & Systems)
- **TurnSystem**:
  - **ActionSystem**: loop over actors starting for Player and trigger **ActorSystem** `action()`.
    - **ActorSystem**: base system for all actors 
      - **PlayerSystem**: command → move/interaction (pickup, open exit). `action()` -> waits for user to press arrow key.
      - **EnemyAISystem**: chase if at least one LOS (Enemy state `resting` -> `chasing`); step or attack.
    - **CombatSystem**: resolve collisions as attacks. If player moves toward enemy, it's not a move, but attack.
  - **ItemSystem**: 
    - if player moves on item cell -> collect item. 
    - if player reach state for autouse item -> autouse (no turn spend, but single per turn).
  - **WinLoseSystem**: check end state each turn/tick.
- **GameLoop**: input → systems → render → repeat until end state.
- **NewGameUseCase**: init `GameState` via `MapGen` + `SpawnPolicy`.
- **SaveGameUseCase / LoadGameUseCase**: serialize/deserialize `GameState`.
- **ConfigLoadUseCase** (optional): load constants from file/env. If it was decided to store config in file.

---

## Infrastructure
- **ConsoleRenderer**: draw `Map`, entities (ASCII), HUD (HP, key), message log.
- **KeyboardInput**: map keys → `InputCommand` (WASD/arrows, `.` wait, `F5` save, `F9` load last save, `Q` save+quit).
- **Persistence**:
  - **ISaveGameRepo** (interface)
    - **FileSaveGameRepo** (impl): saving in TEXT FILE (to have ability for easy open&view saved state from text editor):
      - header { magic "RSLV", version u32=1, endian=LE, saved_at_unix u64 },
      - snapshot of resolved GameRules,
      - full RNG engine state (std::mt19937),
      - world: tick, map (W×H + tiles), entities (player, enemies, items), score,
      - message log (optional, capped),
      - atomic write: write *.tmp → flush/fsync → rename.
      - NOT SERIALIZING: caches, per-tick transient flags, pending intents.
- **Logger**: info/error/debug to stdout/stderr AND to file (compile-time toggles). Different colors in console, logs below the game, flush after turn.
- **RNG Impl**: `StdRng` wrapping `std::mt19937` via `IRng`.
- **Clock** (optional): fixed-step pacing. Some timer that evaluates real time passing (like make turn each 60 seconds or wait)
- **Config** (optional): `assets/config.ini` for sizes/seed/enemy count.

---

## Cross-Cutting Utilities
- **Result<T>/Status**: lightweight error handling for save/load.
- **Grid<T>**: checked 2D container.

---

## Core Interfaces (Domain-Pure Boundaries)
- **IRenderer**: `draw(Map, Entities, HUDInfo, MessageLog)`.
- **IInput**: `poll() -> InputCommand`.
- **ISaveGameRepo**: `save(GameState)`, `load() -> GameState`.
- **IRng**: `next_int(a,b)`, `shuffle(...)`.

---

## Main (executable)
- **main.cpp**:
  - Parse `--seed`.
  - Construct adapters: `ConsoleRenderer`, `KeyboardInput`, `FileSaveGameRepo`, `StdRng`.
  - Run `NewGameUseCase` or `LoadGameUseCase` through `GameLoop`.

---

## Directory Skeleton
```
src/
  domain/
    entities/ (Player, Enemy, Item, Tile, Map, Stats, Position, Glyph)
    services/ (MapGen, LOS, Pathfinding, SpawnPolicy, DamageCalculator, WinLosePolicy)
    rules/    (GameRules, constants)
    core/     (GameState, Rng facade, Value types)
  app/
    systems/  (PlayerSystem, EnemyAISystem, CombatSystem, PickupSystem, WinLoseSystem)
    usecases/ (NewGame, SaveGame, LoadGame, ConfigLoad)
    loop/     (GameLoop, InputCommand)
  infra/
    io_console/        (ConsoleRenderer, KeyboardInput)
    persistence_file/  (FileSaveGameRepo)
    rng_std/           (StdRng)
    config/            (ConfigLoader)
    log/               (Logger)
  main.cpp
tests/
  domain/, app/, infra/
data/
  saves/
build/ (Game executable)
assets/ (optional)
  config.ini 
```

---

## Test Targets
- **Domain**: `MapGen`, `LOS`, `Pathfinding`, `DamageCalculator`, `WinLosePolicy`, `SpawnPolicy` (deterministic via `IRng`).
- **Application**: command→state transitions; AI step outcomes with fixed seed.
- **Infrastructure**: save/load round-trip (golden file).

---

## Notes
- Keep domain pure; treat I/O as plugins behind interfaces.
- Deterministic tests by injecting `IRng` with known seed.
- Single-binary, STL-only; easy to port to another renderer later.

## 4. Implementation Plan

A straight, step-by-step implementation order. Each step names the module(s) it's needed to create/touch and the core logic to implement.

## Milestones
- **M1 (Steps 1–9):** Data model & state compile.
- **M2 (10–15):** Core simulation policies compile & test.
- **M3 (16–21):** Turn systems functional (playable without I/O).
- **M4 (22–29):** I/O adapters + loop (fully playable).
- **M5 (30–37):** Wiring, tests, save/load stable.
- **M6 (38–40):** Docs, CI, release.

### 1) Repo & Build Bootstrapping
- `CMakeLists.txt`, `src/`, `tests/`, `.gitignore`, `README`.
- Set C++20, warnings, out-of-source build, test target.

### 2) Core Value Types (Domain/Core)
- `Position`, `Direction`, `Stats`, `EntityId`, `Glyph`.
- Small helpers: bounds check, equality, hashing.

### 3) Grid Container
- `core/Grid<T>` with size, indexing, safe accessors, iteration.

### 4) Tiles & Map (Domain/Entities)
- `Tile` (wall/floor, blocks_movement, blocks_sight).
- `Map` wrapping `Grid<Tile>` with `in_bounds`, `is_passable`, `is_transparent`.

### 5) Game Rules (Domain/Rules)
- `GameRules` constants: map size, base stats, potion heal, enemy count, drop rates.

### 6) RNG Abstraction
- `IRng` interface in domain; `StdRng` in infra.
- Deterministic seed handling via constructor.

### 7) Entities (Domain/Entities)
- `Player`, `Enemy`, `Item` with `ItemKind{Key, HealthPotion}`.
- Simple POD-style data + getters.

### 8) Entity Registry & IDs (Domain/Core)
- Minimal registry for players/enemies/items with `EntityId` assignment.
- Queries: by position, occupancy checks.

### 9) GameState Aggregate (Domain/Core)
- Holds `Map`, registries, turn counter, victory/defeat flags, score, RNG handle.

### 10) Line-of-Sight (Domain/Services)
- `LOS.can_see(Map, aPos, bPos)` (Bresenham-like traversal against `blocks_sight`).

### 11) Pathfinding (Domain/Services)
- Single-step shortest move toward target on passable tiles (BFS/greedy).

### 12) Spawn Policy (Domain/Services)
- Place player, enemies, items on valid floor cells using `IRng`.

### 13) Map Generation (Domain/Services)
- Rooms-and-corridors or drunkard walk.
- Ensure at least one valid path, place exit (closed/disabled until key).

### 14) Win/Lose Policy (Domain/Services)
- Victory: exit available & reached (key acquired).
- Defeat: player HP ≤ 0.

### 15) Damage / Combat Policy (Domain/Services)
- Resolve attack (hp reduction, min 1), no inventory—auto potion logic later.

### 16) Input Model (App/Loop)
- `InputCommand`: `Move(dir)`, `Wait`, `Save`, `Load`, `Quit`.

### 17) Player System (App/Systems)
- Translate `InputCommand` → attempt move or attack if enemy occupies target.
- Pickup trigger if stepping onto item.

### 18) Enemy AI System (App/Systems)
- State: resting → chasing when `LOS` to player.
- While chasing: step toward player or attack when adjacent.

### 19) Combat System (App/Systems)
- Centralize collision→attack for both player and enemies in turn order.

### 20) Pickup System (App/Systems)
- On entering item tile: collect; if `HealthPotion` and heal needed → auto-use once per turn; `Key` sets `has_key`.

### 21) Win/Lose System (App/Systems)
- Evaluate flags each tick; enable/open exit when key obtained.

### 22) New Game Use-Case (App/Usecases)
- Create `GameState` with rules, RNG, `MapGen`, `SpawnPolicy`.

### 23) Save/Load Interfaces (App/Core & Infra)
- `ISaveGameRepo` with `save(GameState)` / `load() -> GameState`.
- Domain-friendly snapshot contracts (no caches/transients).

### 24) FileSaveGameRepo (Infra/Persistence)
- Text format snapshot (header, rules, rng state, map, entities, turn, flags).
- Atomic write: temp→flush→rename.

### 25) Renderer Interface (App/Core)
- `IRenderer.draw(Map, Entities, HUDInfo, MessageLog)`.

### 26) Console Renderer (Infra/io_console)
- ASCII map, entities, HUD (HP, key), simple message log.

### 27) Input Interface (App/Core)
- `IInput.poll() -> InputCommand`.

### 28) Keyboard Input (Infra/io_console)
- Map WASD/arrows to `Move`, `.` to `Wait`, `F5` Save, `F9` Load, `Q` Quit.

### 29) Game Loop (App/Loop)
- Sequence: read input → player system → enemy AI → combat → pickup → win/lose → render.
- Exit on victory/defeat or `Quit`.

### 30) Main (composition/main.cpp)
- Parse `--seed` / `--load`.
- Wire: `StdRng`, `ConsoleRenderer`, `KeyboardInput`, `FileSaveGameRepo`.
- Start `NewGame` or `LoadGame`, run `GameLoop`.

### 31) Logger (Infra/log)
- Simple info/error with optional file sink; flush each turn.

### 32) Config Loader (Infra/config) [optional]
- `assets/config.ini` for sizes/seed/enemy count; apply to `GameRules`.

### 33) Auto-Use Logic Finalization
- Ensure only one auto potion per turn; no turn cost; capped HP.

### 34) Exit Activation
- Gate exit until key is collected; render exit as inactive/active.

### 35) Tests — Domain
- `MapGen` connectivity, `LOS` correctness, pathfinding step, combat math, win/lose conditions, spawn validity (seeded RNG).

### 36) Tests — Application
- Command→state transitions (move, attack, pickup).
- Enemy AI step outcomes (seeded).
- Save/Load round-trip equivalence (golden).

### 37) Manual Playtest Harness
- CLI toggles: `--seed`, `--map-size`, `--enemies`.
- Scenarios: win via exit, death via combat, save→quit→load.

### 38) Polish & Docs
- README: build/run, controls, rules.
- In-code docs for public interfaces.
- Example save file in `data/saves/` (ignored by Git except example).

### 39) CI (optional)
- Build + unit tests on push.
- Lint/warnings as errors.

### 40) Tag v1.0.0
- Single binary, STL-only, deterministic tests, basic gameplay loop complete.

---

## 5. Testing Strategy

### 5.1 Unit Testing

Making a very simple unit testing framework from scratch (just get bool -> print output) and doing:

* Map generation functions
* Movement logic (valid/invalid moves)
* Combat resolution
* Inventory add/remove

### 5.2 Manual Testing

* Play through all win/lose paths
* Test save/load consistency
* Ensure input validation

---

## 6. Git Workflow

### 6.1 Branching Model

* `main` – stable version (ready to play, stable game versions)
* `dev` – active development
* `feature/<name>` – per-feature branches
* `bugfix/<name>` – per-bug branches

### 6.2 Commit Practice

* Clear commit messages (e.g., `Add player movement logic`)
* Only compilable
* Separate documentation and tests commits
* Pull request (PR) reviews

---

## 7. Documentation

### 7.1 README.md

* Project overview
* Build instructions
* Controls
* Game rules

### 7.2 Code Comments

* Use Doxygen-style comments for public interfaces

---

## 8. Future Enhancements (Optional)

* Configurable map size
* Multiple enemy types
* Fog of war / visibility
* UI improvements (ncurses or SDL later)
* Difficulty scaling

---

## Final Notes

* Keep each system decoupled and testable.
* Commit often and document your progress.
* Follow SOLID principles in architecture.

Happy coding!

---
