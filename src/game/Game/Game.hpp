/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Game class
*/

#pragma once

#include "../Engine.hpp"
#include "protecs/common.hpp"
#include <rtype/components.hpp>
#include <protecs/networking.hpp>
#include <functional>
#include <chrono>
#include <deque>
#include "../Requests/Requests.hpp"
#include <optional>
#include <vector>


namespace RType::Game {
    class Game {
        static constexpr Wrecks::DeltaTime NETWORK_UPDATE_RATE = std::chrono::duration<double>(1.0 / 20.0);
        static constexpr unsigned int      UPDATES_PER_INPUT_SYNC = 10;

        using InternalEvent = std::pair<Components::Player*, Protecs::Networking::Packets::PacketData>;

        public:
            using Event = std::pair<const Components::Player*, Protecs::Networking::Packets::PacketData>;

        private:
            Engine m_Engine;
            Protecs::Tick m_SimulationTick;

            Wrecks::DeltaTime m_FixedUpdateElapsedTime;
            Wrecks::DeltaTime m_NetworkUpdateElapsedTime;
            unsigned int m_NetworkUpdates;

            std::unordered_map<Protecs::Networking::Address, Wrecks::ECS::Entity> m_Players;
            std::deque<InternalEvent> m_IncomingEvents;
            std::deque<Event> m_OutgoingEvents;

            std::function<void(Protecs::Tick)> m_PullEvents;

            // Level and requests of the systems: used by the functions of Level.cpp
            Requests m_Requests;                    // asked by the systems, applied after each fixed update
            std::optional<Wrecks::ECS::Entity> m_Level;  // exists while at least one player is connected

        public:
            Game() noexcept;

            template <typename Fnc>
            void setEventRetriever(Fnc&& fnc) { m_PullEvents = std::move(fnc); }

            void update(Wrecks::DeltaTime dt);

            bool pushEvent(const Protecs::Networking::Address& sender, Protecs::Networking::Packets::PacketData&& data);
            bool popEvent(Event& evt);

            bool registerPlayer(const Protecs::Networking::Address& address, std::string_view name);
            void disconnectPlayer(const Protecs::Networking::Address& address);

        private:
            void handleEvents(Protecs::Tick tick);
            void updateNetwork();

            void makePlayer(Wrecks::ECS::Entity entity);

            // Level and requests of the systems: implemented in Level.cpp
            void applyRequests();
            void spawnBody(const SpawnRequest& request);
            void initPlayerBody(Wrecks::ECS::Entity player);
            void startLevel();
            void endLevel();

            Wrecks::ECS::Entity findEntity(Protecs::NetworkEntity networkId) const;
            void broadcastPacket(Protecs::Networking::Packets::PacketData&& packet);

            template <Wrecks::Utils::ContainerContainsType<Protecs::Components::Manager> C>
            void sendSharedComponent(Wrecks::ECS::Entity entity)
            {
                const auto& networkId = m_Engine.get<Components::NetworkID>()[entity];

                broadcastPacket(Protecs::Networking::Packets::EntityComponentData{
                    .simulationTick = m_SimulationTick,
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
                    void operator()(const Protecs::Networking::Packets::TickSync& data);
            };
    };
}
