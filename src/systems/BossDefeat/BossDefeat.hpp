/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** BossDefeat system
*/

#pragma once

#include <rtype/components.hpp>
#include "../../game/Requests/Requests.hpp"
#include "../../game/Rules.hpp"
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * A boss killed (Health at 0, after Collisions): the enemies
     * and their missiles go away, and the level is cleared.
     */
    class BossDefeat {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 120,
            };

        private:
            void* m_Engine;
            Game::Requests* m_Requests;

        public:
            BossDefeat() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Body>())
                    .set(Components::Manager::index<Components::Boss>())
                    .set(Components::Manager::index<Protecs::Components::Health>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }
            constexpr void setRequests(Game::Requests& requests) noexcept { m_Requests = &requests; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
