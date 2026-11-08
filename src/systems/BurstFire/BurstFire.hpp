/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** BurstFire system
*/

#pragma once

#include <rtype/components.hpp>
#include "../../game/Requests/Requests.hpp"
#include "../../game/Rules.hpp"
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * Entities with AimedBursts fire their bursts at the nearest
     * player still in the game (not while entering, with StopAt).
     */
    class BurstFire {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 45,
            };

        private:
            void* m_Engine;
            Game::Requests* m_Requests;

        public:
            BurstFire() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Transform>())
                    .set(Components::Manager::index<Components::Body>())
                    .set(Components::Manager::index<Components::AimedBursts>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }
            constexpr void setRequests(Game::Requests& requests) noexcept { m_Requests = &requests; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);

        private:
            void shootAtNearestPlayer(Rules::Vec2D from, float speed);
    };
}
