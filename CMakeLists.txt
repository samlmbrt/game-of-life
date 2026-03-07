cmake_minimum_required(VERSION 3.28)
project(game-of-life LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics REQUIRED)

add_executable(game-of-life src/main.cpp src/grid.cpp)
target_link_libraries(game-of-life PRIVATE SFML::Graphics)

target_compile_options(game-of-life PRIVATE
    -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Werror
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(game-of-life PRIVATE
        -fsanitize=address,undefined -fno-omit-frame-pointer
    )
    target_link_options(game-of-life PRIVATE
        -fsanitize=address,undefined
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(game-of-life PRIVATE
        -march=native -flto
    )
    target_link_options(game-of-life PRIVATE
        -flto
    )
endif()
