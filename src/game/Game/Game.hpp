/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Game class
*/

#pragma once

#include "../Engine.hpp"
#include <rtype/components.hpp>
#include <protecs/networking.hpp>
#include <deque>


namespace RType::Game {
    class Game {
        using InternalEvent = std::pair<Components::Player*, Protecs::Networking::Packets::PacketData>;

        public:
            using Event = std::pair<const Components::Player*, Protecs::Networking::Packets::PacketData>;

        private:
            Engine m_Engine;
            std::unordered_map<Protecs::Networking::Address, Wrecks::ECS::Entity> m_Players;
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
                Components::Player& m_Player;

                public:
                    EventHandler(Game& game, Components::Player& player) noexcept;

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
