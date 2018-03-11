cmake_minimum_required(VERSION 3.10)

project(uuid)
set(CMAKE_CXX_STANDARD 17)

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

target_link_libraries(uuid
PUBLIC
    aes
    b64
)
