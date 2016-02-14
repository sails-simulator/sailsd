cmake_minimum_required(VERSION 2.6)

project(sailsd)

set (SAILS_VERSION_MAJOR 0)
set (SAILS_VERSION_MINOR 1)

set(INSTALLED_RESOURCES "${DATADIR}/sailsd")

add_executable(sailsd sailsd.c)

find_package(PkgConfig)

pkg_search_module(JANSSON REQUIRED jansson)

target_link_libraries(sailsd m
                             pthread)
target_link_libraries(sailsd ${JANSSON_LIBRARIES})
#target_include_directories(sailds PUBLIC ${JANSSON_INCLUDE_DIRS})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug build.")
    add_definitions("-Wall -Werror -g")
endif()

install(TARGETS sailsd
        RUNTIME
        DESTINATION bin
    )
