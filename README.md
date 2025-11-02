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
