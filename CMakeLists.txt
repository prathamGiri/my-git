cmake_minimum_required(VERSION 3.14)
project(mygit CXX)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(mygit_lib 
    src/core/sha1.cpp
    src/core/zlib.cpp
    src/commands/hash_object.cpp
)
target_include_directories(mygit_lib
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

if(MSVC)
    target_compile_options(mygit_lib PRIVATE /W4 /permissive-)
else()
    target_compile_options(mygit_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(mygit app/main.cpp)
target_link_libraries(mygit PRIVATE mygit_lib)

find_package(ZLIB REQUIRED)
target_link_libraries(mygit PRIVATE ZLIB::ZLIB)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests 
    tests/sha1_test.cpp
    tests/hash_object_test.cpp
)
target_link_libraries(tests PRIVATE mygit_lib GTest::gtest_main)
target_compile_definitions(tests PRIVATE
    TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/data")
    
include(GoogleTest)
gtest_discover_tests(tests)