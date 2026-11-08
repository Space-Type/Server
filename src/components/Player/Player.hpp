/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Player component
*/

#pragma once

#include <protecs/networking.hpp>
#include <chrono>
#include <string>


namespace RType::Components {
    struct Player {
        Protecs::Networking::Address address;
        std::string name;
        std::chrono::steady_clock::time_point lastSeen = std::chrono::steady_clock::now();  // last packet received from it
    };
}
