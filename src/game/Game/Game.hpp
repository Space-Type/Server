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
        static constexpr auto FIXED_UPDATE_RATE = Wrecks::DeltaTime(2.0 / 120.0);

        using InternalEvent = std::pair<Components::Player*, Protecs::Networking::Packets::PacketData>;

        public:
            using Event = std::pair<const Components::Player*, Protecs::Networking::Packets::PacketData>;

        private:
            Engine m_Engine;
            Wrecks::DeltaTime m_AccumulatedDelta;

            std::unordered_map<Protecs::Networking::Address, Wrecks::ECS::Entity> m_Players;
            std::deque<InternalEvent> m_IncomingEvents;
            std::deque<Event> m_OutgoingEvents;

        public:
            Game() noexcept;

            void update(Wrecks::DeltaTime dt);

            bool pushEvent(const Protecs::Networking::Address& sender, Protecs::Networking::Packets::PacketData&& data);
            bool popEvent(Event& evt);

            bool registerPlayer(const Protecs::Networking::Address& address, std::string_view name);
            void disconnectPlayer(const Protecs::Networking::Address& address);

            template <Wrecks::Utils::ContainerContainsType<Protecs::Components::Manager> C>
            void sendSharedComponent(Wrecks::ECS::Entity entity)
            {
                const auto& networkId = m_Engine.get<Components::NetworkID>()[entity];

                broadcastPacket(Protecs::Networking::Packets::EntityComponentData{
                    .entity = networkId.id,
                    .component = m_Engine.get<C>()[entity],
                });
            }

            template <Wrecks::Utils::ContainerContainsType<Protecs::Components::Manager> C>
            void addSharedComponent(Wrecks::ECS::Entity entity, C&& data)
            {
                m_Engine.addComponent(entity, std::move(data));
                sendSharedComponent<C>(entity);
            }

        private:
            void handleEvents();
            void broadcastPacket(Protecs::Networking::Packets::PacketData&& packet);

        private:
            class EventHandler {
                Game& m_Game;
                Components::Player& m_Player;

                public:
                    EventHandler(Game& game, Components::Player& player) noexcept;

                    void operator()(const Protecs::Networking::Packets::EntityComponentData& data);
                    void operator()(const Protecs::Networking::Packets::EntityCreate& data);
                    void operator()(const Protecs::Networking::Packets::EntityDelete& data);
                    void operator()(const Protecs::Networking::Packets::EntityList& data);
                    void operator()(const Protecs::Networking::Packets::EntityRemoveComponent& data);
                    void operator()(const Protecs::Networking::Packets::EntitySyncComponents& data);
                    void operator()(const Protecs::Networking::Packets::Ping& data);
                    void operator()(const Protecs::Networking::Packets::PlayerId& data);
                    void operator()(const Protecs::Networking::Packets::PlayerInput& data);
                    void operator()(const Protecs::Networking::Packets::PlayerJoin& data);
                    void operator()(const Protecs::Networking::Packets::PlayerLeave& data);
            };
    };
}
