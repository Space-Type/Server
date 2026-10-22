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
    class Player;

    class EventHandler {
        Game& m_Game;
        Player& m_Player;

        public:
            EventHandler(Game& game, Player& player) noexcept;

            void operator()(const Protecs::Networking::Packets::EntityAddComponent& data);
            void operator()(const Protecs::Networking::Packets::EntityComponentData& data);
            void operator()(const Protecs::Networking::Packets::EntityRemoveComponent& data);
            void operator()(const Protecs::Networking::Packets::EntitySyncComponents& data);
            void operator()(const Protecs::Networking::Packets::Ping& data);
            void operator()(const Protecs::Networking::Packets::PlayerInput& data);
            void operator()(const Protecs::Networking::Packets::PlayerJoin& data);
            void operator()(const Protecs::Networking::Packets::PlayerLeave& data);
    };
}
