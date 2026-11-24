cmake_minimum_required(VERSION 3.16)
project(mygit VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(mygit_lib
    src/sha1.cpp
)

target_include_directories(mygit_lib
    PUBLIC  ${PROJECT_SOURCE_DIR}/include   
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)
target_compile_options(mygit_lib PRIVATE -Wall -Wextra -Wpedantic)

add_executable(mygit app/main.cpp)
target_link_libraries(mygit PRIVATE mygit_lib)

# ---- Tests ----
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests
    tests/sha1_test.cpp
)
target_link_libraries(tests PRIVATE mygit_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)