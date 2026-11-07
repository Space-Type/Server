/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Collisions system
*/

#pragma once

#include <rtype/components.hpp>
#include "../../game/Requests/Requests.hpp"
#include "../../game/Rules.hpp"
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * Player missiles and beams damage enemies (score to the
     * shooter for a kill); enemies and their missiles cost a
     * life to players, who come back at their start
     * invulnerable for a moment, or are out with no life left.
     * Runs after PlayerBounds.
     */
    class Collisions {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 110,
            };

        private:
            void* m_Engine;
            Game::Requests* m_Requests;

        public:
            Collisions() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Transform>())
                    .set(Components::Manager::index<Components::Body>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }
            constexpr void setRequests(Game::Requests& requests) noexcept { m_Requests = &requests; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);

        private:
            void hitWithShot(Wrecks::ECS::Entity shot);
            void hitPlayer(Wrecks::ECS::Entity player);
    };
}
