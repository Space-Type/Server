##
## EPITECH PROJECT, 2026
## Project - RType
## File description:
## Main project build
## file
##

cmake_minimum_required(VERSION 3.15)

project(RType-Server VERSION 1.0
                     DESCRIPTION "R-Type Game Server - R-Type project"
                     HOMEPAGE_URL "https://github.com/Space-Type/Server"
                     LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)


# ----------- Project Dependencies ----------- #

if (NOT TARGET wrecks)
    add_subdirectory(lib/wrecks)
endif()
if (NOT TARGET protecs)
    add_subdirectory(lib/protecs)
endif()


# --------------- Project Build -------------- #

set(SOURCES
    src/components/Player/Player.cpp
    src/components/Player/Player.hpp
    src/components/NetworkID/NetworkID.cpp
    src/components/NetworkID/NetworkID.hpp
    src/components/Manager.hpp
    src/game/Game/Game.cpp
    src/game/Game/Game.hpp
    src/game/Server/Server.cpp
    src/game/Server/Server.hpp
    src/game/Engine.hpp
    src/systems/Tuple.hpp
    src/threads/ThreadQueue.hpp
    src/utils/Logger/Logger.cpp
    src/utils/Logger/Logger.hpp
    src/utils/SignalHandler.hpp
    src/main.cpp)

add_executable(r-type_server ${SOURCES})

target_include_directories(r-type_server PUBLIC include
                                         PUBLIC lib/wrecks/include
                                         PUBLIC lib/protecs/include)

target_link_libraries(r-type_server PUBLIC wrecks
                                    PUBLIC protecs)
