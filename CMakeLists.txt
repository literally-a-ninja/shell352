cmake_minimum_required(VERSION 3.1)

project(shell352)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE LIB_BUONGIORNO_SOURCES CONFIGURE_DEPENDS "src/buongiorno/*.c")
file(GLOB_RECURSE LIB_BUONGIORNO_HEADERS CONFIGURE_DEPENDS "src/buongiorno/*.h")

add_library(buongiorno STATIC ${LIB_BUONGIORNO_SOURCES})
target_sources(buongiorno PRIVATE ${LIB_BUONGIORNO_HEADERS})

add_executable(shell352 src/main.c)
target_link_libraries(shell352 buongiorno)
