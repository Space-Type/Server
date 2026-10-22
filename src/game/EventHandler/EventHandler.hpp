/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** EventHandler class
*/

#pragma once

#include <protecs/networking.hpp>


namespace RType::Game {
    class Game;

    class EventHandler {
        const Protecs::Networking::Address& m_Sender;
        Game& m_Game;

        public:
            EventHandler(Game& game, const Protecs::Networking::Address& sender) noexcept;

            void operator()(const Protecs::Networking::Packets::EntityAddComponent& data);
            void operator()(const Protecs::Networking::Packets::EntityComponentData& data);
            void operator()(const Protecs::Networking::Packets::EntityRemoveComponent& data);
            void operator()(const Protecs::Networking::Packets::EntitySyncComponents& data);
            void operator()(const Protecs::Networking::Packets::PlayerInput& data);
    };
}
