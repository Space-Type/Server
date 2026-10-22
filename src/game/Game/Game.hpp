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

        private:
            void handleEvents();
    };
}
