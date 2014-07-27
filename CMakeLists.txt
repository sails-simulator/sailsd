project(sails C)
cmake_minimum_required(VERSION 2.6)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(sails sail_application.c)
target_link_libraries(sails ${GTK3_LIBRARIES})
