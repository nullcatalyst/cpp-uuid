cmake_minimum_required(VERSION 3.10)

project(uuid)

add_library(uuid "")

target_include_directories(uuid
PUBLIC
    include
PRIVATE
    src
)

target_sources(uuid
PRIVATE
    src/uuid.cpp
)

target_compile_options(uuid
PRIVATE
    -std=c++14
)

target_link_libraries(uuid
PUBLIC
    b64
)
