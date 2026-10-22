/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Game class
*/

#pragma once

#include "../Engine.hpp"
#include "../Player/Player.hpp"
#include <protecs/networking.hpp>
#include <deque>


namespace RType::Game {
    class Game {
        using InternalEvent = std::pair<Player*, Protecs::Networking::Packets::PacketData>;

        public:
            using Event = std::pair<const Player*, Protecs::Networking::Packets::PacketData>;

        private:
            Engine m_Engine;
            std::unordered_map<Protecs::Networking::Address, Player> m_Players;
            std::deque<InternalEvent> m_IncomingEvents;
            std::deque<Event> m_OutgoingEvents;

        public:
            Game() noexcept;

            void update();

            bool pushEvent(const Protecs::Networking::Address& sender, Protecs::Networking::Packets::PacketData&& data);
            bool popEvent(Event& evt);

            bool registerPlayer(const Protecs::Networking::Address& address, std::string_view name);
            void disconnectPlayer(const Protecs::Networking::Address& address);

        private:
            void handleEvents();

        private:
            class EventHandler {
                Game& m_Game;
                Player& m_Player;

                public:
                    EventHandler(Game& game, Player& player) noexcept;

                    void operator()(const Protecs::Networking::Packets::EntityAddComponent& data);
                    void operator()(const Protecs::Networking::Packets::EntityComponentData& data);
                    void operator()(const Protecs::Networking::Packets::EntityCreate& data);
                    void operator()(const Protecs::Networking::Packets::EntityDelete& data);
                    void operator()(const Protecs::Networking::Packets::EntityRemoveComponent& data);
                    void operator()(const Protecs::Networking::Packets::EntitySyncComponents& data);
                    void operator()(const Protecs::Networking::Packets::Ping& data);
                    void operator()(const Protecs::Networking::Packets::PlayerInput& data);
                    void operator()(const Protecs::Networking::Packets::PlayerJoin& data);
                    void operator()(const Protecs::Networking::Packets::PlayerLeave& data);
            };
    };
}
