cmake_minimum_required(VERSION 2.6)
project(Sails C)
add_executable(sails sail_application.c
                     sail_boat.c
                     sail_boat_draw.c
                     sail_overlay.c
                     sail_physics.c
                     sail_view.c
                     sail_viewstate.c
                     sail_wind.c
    )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(DATADIR "${CMAKE_INSTALL_PREFIX}/share")
set(RESOURCESDIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")

find_package(PkgConfig REQUIRED)
find_package(Cairo REQUIRED)
find_package(GTK3 REQUIRED)
find_package(Rsvg REQUIRED)

include_directories(${PROJECT_BINARY_DIR})
include_directories(${GTK3_INCLUDE_DIRS})
include_directories(${CAIRO_INCLUDE_DIR})
include_directories(${RSVG_INCLUDE_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER} "-Wall -Werror -O2 -g")

target_link_libraries(sails m)
target_link_libraries(sails ${GTK3_LIBRARIES}
                            ${RSVG_LIBRARIES}
                            ${CAIRO_LIBRARIES}
    )

install(TARGETS sails
        RUNTIME
        DESTINATION bin
    )

install(FILES ${RESOURCESDIR}/sails.desktop
        DESTINATION ${DATADIR}/applications/)

install(FILES
        ${RESOURCESDIR}/hull.svg
        ${RESOURCESDIR}/sail.svg
        ${RESOURCESDIR}/rudder.svg
        DESTINATION ${DATADIR}/sails/)
