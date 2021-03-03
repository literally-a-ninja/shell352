cmake_minimum_required(VERSION 3.1)

project(shell352)

find_package(Threads REQUIRED)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

file(GLOB_RECURSE LIB_BUONGIORNO_SOURCES CONFIGURE_DEPENDS "src/buongiorno/*.c")
file(GLOB_RECURSE LIB_BUONGIORNO_HEADERS CONFIGURE_DEPENDS "src/buongiorno/*.h")

add_library(buongiorno STATIC ${LIB_BUONGIORNO_SOURCES})
target_sources(buongiorno PRIVATE ${LIB_BUONGIORNO_HEADERS})
target_link_libraries(buongiorno PRIVATE Threads::Threads)

add_compile_definitions(shell352 DEBUG)
add_executable(shell352 src/main.c)
target_link_libraries(shell352 buongiorno)
