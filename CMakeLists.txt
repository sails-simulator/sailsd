cmake_minimum_required(VERSION 2.6)

project(sailsd)

set (SAILS_VERSION_MAJOR 0)
set (SAILS_VERSION_MINOR 1)

set(INSTALLED_RESOURCES "${DATADIR}/sailsd")

add_executable(sailsd sailsd.c)

target_link_libraries(sailsd m)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug build.")
    add_definitions("-Wall -Werror -g")
ENDIF()

install(TARGETS sailsd
        RUNTIME
        DESTINATION bin
    )
