/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** EventHandler class
*/

#pragma once

#include <protecs/networking/packets.hpp>


namespace RType::Game {
    class EventHandler {
        public:
            EventHandler() noexcept;

            void operator()(const Protecs::Networking::Packets::EntityPosition& data);
            void operator()(const Protecs::Networking::Packets::PlayerInput& data);
    };
}
