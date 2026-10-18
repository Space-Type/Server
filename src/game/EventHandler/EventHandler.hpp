/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** EventHandler class
*/

#pragma once

#include <rtype/networking/packets.hpp>


namespace RType::Game {
    class EventHandler {
        public:
            EventHandler() noexcept;

            void operator()(const Networking::Packets::EntityPosition& data);
    };
}
