
cmake_minimum_required(VERSION 3.20)
project(labyrinth VERSION 0.1.0 LANGUAGES CXX)

# Options
option(LABYRINTH_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(LABYRINTH_ENABLE_SANITIZERS "Enable address/undefined sanitizers (non-MSVC)" ON)
option(LABYRINTH_BUILD_TESTS "Build unit tests" ON)

# C++ standard and global compile features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output dirs (out-of-source builds recommended)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Warnings
set(PROJ_WARNINGS -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wnon-virtual-dtor)
if(LABYRINTH_WARNINGS_AS_ERRORS)
    list(APPEND PROJ_WARNINGS -Werror)
endif()

# Sanitizers
if(LABYRINTH_ENABLE_SANITIZERS AND NOT MSVC)
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()

# Library target (core/game logic will live here progressively)
add_library(labyrinth_lib
    src/domain/core/version.cpp
)
target_include_directories(labyrinth_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(labyrinth_lib PRIVATE ${PROJ_WARNINGS})

# Executable target (wires the app; minimal now)
add_executable(labyrinth
    src/main.cpp
)
target_link_libraries(labyrinth PRIVATE labyrinth_lib)
target_compile_options(labyrinth PRIVATE ${PROJ_WARNINGS})

# Enable testing if requested
if(LABYRINTH_BUILD_TESTS)
    include(CTest)
    if(BUILD_TESTING)
        add_executable(tests_sanity
            tests/test_sanity.cpp
        )
        target_link_libraries(tests_sanity PRIVATE labyrinth_lib)
        target_compile_options(tests_sanity PRIVATE ${PROJ_WARNINGS})
        add_test(NAME Sanity COMMAND tests_sanity)
    endif()
endif()

# Install (optional basic rules)
include(GNUInstallDirs)
install(TARGETS labyrinth labyrinth_lib
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY src/ DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")
