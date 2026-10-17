##
## EPITECH PROJECT, 2026
## Project - R-Type
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

add_subdirectory(lib/wrecks)

set(SOURCES
    src/main.cpp)

add_executable(r-type_server ${SOURCES})

target_include_directories(r-type_server PUBLIC include
                                         PUBLIC lib/wrecks/include)

target_link_libraries(r-type_server PUBLIC wrecks)
