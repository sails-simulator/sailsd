cmake_minimum_required(VERSION 2.6)
project(sails C)
add_executable(sails sail_application.c
                     sail_boat.c
                     sail_overlay.c
                     sail_physics.c
                     sail_view.c
                     sail_viewstate.c
    )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(PkgConfig REQUIRED)
find_package(Cairo REQUIRED)
find_package(GTK3 REQUIRED)
pkg_check_modules(RSVG REQUIRED librsvg-2.0)
pkg_check_modules(CAIRO REQUIRED cairo)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${GTK3_INCLUDE_DIRS}")
include_directories("${CAIRO_INCLUDE_DIR}")
include_directories("${RSVG_INCLUDE_DIRS}")

add_definitions("${GTK3_CFLAGS_OTHER}")

target_link_libraries(sails m)
target_link_libraries(sails "${GTK3_LIBRARIES}"
                            "${RSVG_LIBRARIES}"
                            "${CAIRO_LIBRARIES}"
    )
