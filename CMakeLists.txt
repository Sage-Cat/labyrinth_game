# Build three static libs by layer:
#   labyrinth_domain  (pure gameplay logic)
#   labyrinth_app     (use-cases/systems; depends on domain)
#   labyrinth_infra   (adapters; depends on app + domain)

# ---------- Domain ----------
file(GLOB_RECURSE DOMAIN_SRC CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/domain/core/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/domain/core/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/domain/entities/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/domain/entities/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/domain/rules/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/domain/rules/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/domain/services/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/domain/services/*.hpp"
)
add_library(labyrinth_domain STATIC ${DOMAIN_SRC})
target_include_directories(labyrinth_domain
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${DOMAIN_SRC})

#---------- Application ----------
file(GLOB_RECURSE APP_SRC CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/app/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/app/*.hpp"
)
add_library(labyrinth_app STATIC ${APP_SRC})
target_include_directories(labyrinth_app
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(labyrinth_app
  PUBLIC
    labyrinth_domain
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${APP_SRC})

# ---------- Infrastructure ----------
file(GLOB_RECURSE INFRA_SRC CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/infra/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/infra/*.hpp"
)
add_library(labyrinth_infra STATIC ${INFRA_SRC})
target_include_directories(labyrinth_infra
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(labyrinth_infra
  PUBLIC
    labyrinth_app
    labyrinth_domain
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${INFRA_SRC})
