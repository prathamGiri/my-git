cmake_minimum_required(VERSION 3.16)
project(mygit VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mygit 
    src/main.cpp
    src/SHA1.cpp
    src/MSA.cpp
)

target_include_directories(mygit PUBLIC include)