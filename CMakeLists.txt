cmake_minimum_required(VERSION 2.6)
project(sails C)
add_executable(sails sail_application.c
                     sail_boat.c
                     sail_overlay.c
                     sail_physics.c
                     sail_view.c
                     sail_viewstate.c
    )

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(RSVG REQUIRED librsvg-2.0)
PKG_CHECK_MODULES(CAIRO REQUIRED cairo)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${GTK3_INCLUDE_DIRS}")
include_directories("${CAIRO_INCLUDE_DIRS}")
include_directories("${RSVG_INCLUDE_DIRS}")

add_definitions("${GTK3_CFLAGS_OTHER}")

target_link_libraries(sails m)
target_link_libraries(sails "${GTK3_LIBRARIES}")
target_link_libraries(sails "${RSVG_LIBRARIES}")
target_link_libraries(sails "${CAIRO_LIBRARIES}")
