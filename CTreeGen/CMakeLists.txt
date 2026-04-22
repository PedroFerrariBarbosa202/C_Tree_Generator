cmake_minimum_required(VERSION 3.10)

project(CTreeGen)

add_executable(c_tree
    src/main.c
    src/c_tree.c
    src/random.c
)

target_include_directories(c_tree PRIVATE 
  src
  config
)

