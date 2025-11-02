# Collect root-level tests (e.g., test_sanity.cpp)
file(GLOB ROOT_TESTS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

foreach(t ${ROOT_TESTS})
  get_filename_component(name "${t}" NAME_WE)
  add_executable(${name} ${t})
  target_link_libraries(${name} PRIVATE labyrinth_domain)
  add_test(NAME ${name} COMMAND ${name})
endforeach()

# Collect domain tests under tests/domain/*.cpp
file(GLOB DOMAIN_TESTS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/domain/*.cpp"
)

foreach(t ${DOMAIN_TESTS})
  get_filename_component(name "${t}" NAME_WE)
  add_executable(${name} ${t})
  target_link_libraries(${name} PRIVATE labyrinth_domain)
  add_test(NAME ${name} COMMAND ${name})
endforeach()

# ---- Infra tests ----
file(GLOB INFRA_TESTS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/infra/*.cpp"
)

foreach(t ${INFRA_TESTS})
  get_filename_component(name "${t}" NAME_WE)
  add_executable(${name} ${t})
  # Infra depends on app + domain; link infra to pull both
  target_link_libraries(${name} PRIVATE labyrinth_infra)
  add_test(NAME ${name} COMMAND ${name})
endforeach()

# ---- App tests ----
file(GLOB APP_TESTS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/app/*.cpp"
)
foreach(t ${APP_TESTS})
  get_filename_component(name "${t}" NAME_WE)
  add_executable(${name} ${t})
  target_link_libraries(${name} PRIVATE labyrinth_app)
  add_test(NAME ${name} COMMAND ${name})
endforeach()