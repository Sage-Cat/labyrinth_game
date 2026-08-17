# labyrinth

Console rogue-like in modern C++20. This repository currently contains **Step 1: Repo & Build Bootstrapping**.

## Build (out-of-source recommended)
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

## Run
```bash
./build/bin/labyrinth
```

Default symbol set is ASCII. You can switch at runtime:
```bash
./build/bin/labyrinth --symbols=ascii
./build/bin/labyrinth --symbols=unicode
```
Or via env var:
```bash
LABYRINTH_SYMBOLS=ascii ./build/bin/labyrinth
```

### Controls (MVP)
- `w` `a` `s` `d`: move player (no Enter needed in terminal)
- Arrow keys: move player
- `.`: wait one turn
- `q`: quit

## Tests
```bash
ctest --test-dir build --output-on-failure
```

## Options
- `LABYRINTH_WARNINGS_AS_ERRORS=ON` — treat warnings as errors.
- `LABYRINTH_ENABLE_SANITIZERS=ON` — enable ASan/UBSan on non-MSVC.
- `LABYRINTH_BUILD_TESTS=OFF` — skip tests.

## Layout
See `src/` and `tests/` skeleton matching the implementation plan.
