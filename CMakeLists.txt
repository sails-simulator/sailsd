cmake_minimum_required(VERSION 2.6)

project(sailsd)

set (SAILS_VERSION_MAJOR 0)
set (SAILS_VERSION_MINOR 1)

set(INSTALLED_RESOURCES "${DATADIR}/sailsd")

add_executable(sailsd sailsd.c)

target_link_libraries(sailsd m)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug build.")
    if(MSVC)
      # Force to always compile with W4
      if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
      else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
      endif()
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -g")
    endif()
endif()

install(TARGETS sailsd
        RUNTIME
        DESTINATION bin
    )
