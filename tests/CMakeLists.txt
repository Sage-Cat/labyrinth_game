function(labyrinth_configure_test target)
  target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/includes)
  if(MSVC)
    target_compile_options(${target} PRIVATE /UNDEBUG)
  else()
    target_compile_options(${target} PRIVATE -UNDEBUG)
  endif()
  add_test(NAME ${target} COMMAND ${target})
endfunction()

# ---- Root tests ----
file(GLOB ROOT_TESTS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

foreach(t ${ROOT_TESTS})
  get_filename_component(name "${t}" NAME_WE)
  add_executable(${name} ${t})
  target_link_libraries(${name} PRIVATE labyrinth_domain)
  labyrinth_configure_test(${name})
endforeach()

# ---- Domain tests ----
file(GLOB DOMAIN_TESTS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/domain/*.cpp"
)

foreach(t ${DOMAIN_TESTS})
  get_filename_component(name "${t}" NAME_WE)
  add_executable(${name} ${t})
  target_link_libraries(${name} PRIVATE labyrinth_domain)
  labyrinth_configure_test(${name})
endforeach()

# ---- Infra tests ----
file(GLOB INFRA_TESTS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/infra/*.cpp"
)

foreach(t ${INFRA_TESTS})
  get_filename_component(name "${t}" NAME_WE)
  add_executable(${name} ${t})
  target_link_libraries(${name} PRIVATE labyrinth_infra)
  labyrinth_configure_test(${name})
endforeach()

# ---- App tests ----
file(GLOB APP_TESTS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/app/*.cpp"
)
foreach(t ${APP_TESTS})
  get_filename_component(name "${t}" NAME_WE)
  add_executable(${name} ${t})
  target_link_libraries(${name} PRIVATE labyrinth_app)
  labyrinth_configure_test(${name})
endforeach()
