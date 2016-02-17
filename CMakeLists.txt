cmake_minimum_required(VERSION 2.6)

project(sailsd)

set (SAILS_VERSION_MAJOR 0)
set (SAILS_VERSION_MINOR 1)

set(INSTALLED_RESOURCES "${DATADIR}/sailsd")

find_package(PkgConfig)

pkg_search_module(JANSSON REQUIRED jansson)
include_directories(${JANSSON_INCLUDE_DIRS})
link_libraries(${JANSSON_LIBRARIES})

add_executable(sailsd sailsd.c)

target_link_libraries(sailsd m
                             pthread)

set_property(TARGET sailsd PROPERTY C_STANDARD 99)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug build.")
    add_definitions("-Wall -Werror -g")
endif()

install(TARGETS sailsd
        RUNTIME
        DESTINATION bin
    )
