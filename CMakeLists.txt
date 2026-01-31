cmake_minimum_required(VERSION 3.23)

if(NOT DEFINED CMAKE_CXX_COMPILER)
  find_program(GCC_CXX NAMES g++-15 g++-14 g++-13)
  if(GCC_CXX)
    set(CMAKE_CXX_COMPILER "${GCC_CXX}" CACHE FILEPATH "C++ compiler")
  endif()
endif()

project(blackjack LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(blackjack
  src/card.cpp
  src/deck.cpp
  src/hand.cpp
  src/player.cpp
  src/game.cpp
  src/terminal_controller.cpp
  src/terminal_game_output.cpp
  src/main.cpp
)
target_include_directories(blackjack PRIVATE src)
