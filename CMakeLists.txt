cmake_minimum_required(VERSION 3.10)

project(uuid)

add_library(uuid "")

option(USE_ARC4RANDOM "Use `arc4random()` to fill random values rather than standard C++ methods." TRUE)

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

if(USE_ARC4RANDOM)
    target_compile_definitions(uuid
    PUBLIC
        USE_ARC4RANDOM
    )
endif()

target_compile_options(uuid
PRIVATE
    -std=c++14
)

target_link_libraries(uuid
PUBLIC
    b64
)
