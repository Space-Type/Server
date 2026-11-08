/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** ForceControl system
*/

#pragma once

#include <rtype/components.hpp>
#include "../../game/Requests/Requests.hpp"
#include "../../game/Rules.hpp"
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * Moves the Forces: an attached one stays at the front or
     * the back of its ship; the Force button launches it (it
     * flies, then floats at the screen edge or after a while)
     * or recalls it (it flies back). Touching its ship, it
     * attaches on that side. Lost with a life of its player,
     * or with its player. Runs after PlayerBounds.
     */
    class ForceControl {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 105,
            };

        private:
            void* m_Engine;
            Game::Requests* m_Requests;

        public:
            ForceControl() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Transform>())
                    .set(Components::Manager::index<Components::Body>())
                    .set(Components::Manager::index<Components::Force>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }
            constexpr void setRequests(Game::Requests& requests) noexcept { m_Requests = &requests; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
