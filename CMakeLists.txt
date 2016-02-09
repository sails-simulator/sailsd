cmake_minimum_required(VERSION 2.6)

project(sailsd)

set (SAILS_VERSION_MAJOR 0)
set (SAILS_VERSION_MINOR 1)

set(INSTALLED_RESOURCES "${DATADIR}/sailsd")

find_package(PkgConfig REQUIRED)

add_definitions("-Wall -Werror -g")

add_executable(sailsd sailsd.c)

target_link_libraries(sailsd m)

install(TARGETS sailsd
        RUNTIME
        DESTINATION bin
    )
