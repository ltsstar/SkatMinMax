cmake_minimum_required(VERSION 3.23)
project(SkatMinMax)

set(CMAKE_CXX_STANDARD 23)
find_package(Catch2 3 REQUIRED)

add_executable(SkatMinMax main.cpp skat.cpp minmax.cpp)

add_executable(tests tests.cpp skat.cpp minmax.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")