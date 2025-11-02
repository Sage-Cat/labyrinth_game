cmake_minimum_required(VERSION 3.20)

# ---- Project ----
project(labyrinth
  VERSION 0.1.0
  LANGUAGES CXX)

# ---- Options ----
option(LABYRINTH_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(LABYRINTH_ENABLE_SANITIZERS  "Enable ASan/UBSan (non-MSVC)" ON)
option(LABYRINTH_BUILD_TESTS        "Build unit tests" ON)

# ---- C++ standard ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Output dirs ----
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---- Warnings ----
if(MSVC)
  add_compile_options(/W4 /permissive- /Zc:__cplusplus
    $<$<BOOL:${LABYRINTH_WARNINGS_AS_ERRORS}>:/WX>)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wimplicit-fallthrough
    $<$<BOOL:${LABYRINTH_WARNINGS_AS_ERRORS}>:-Werror>)
endif()

# ---- Sanitizers (non-MSVC) ----
if(LABYRINTH_ENABLE_SANITIZERS AND NOT MSVC)
  add_compile_options(-fsanitize=address,undefined)
  add_link_options(-fsanitize=address,undefined)
endif()

# ---- Expose version to code ----
add_compile_definitions(
  LABYRINTH_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
  LABYRINTH_VERSION_MINOR=${PROJECT_VERSION_MINOR}
  LABYRINTH_VERSION_PATCH=${PROJECT_VERSION_PATCH}
  LABYRINTH_VERSION_STRING="${PROJECT_VERSION}"
)

# ---- Build layered libs ----
add_subdirectory(src)

# ---- Executable (only main.cpp) ----
add_executable(labyrinth src/main.cpp)
# link: exe -> infra (adapters) + app (use-cases); domain comes transitively
target_link_libraries(labyrinth PRIVATE labyrinth_infra labyrinth_app)

# ---- Tests ----
if(LABYRINTH_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

