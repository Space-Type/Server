/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** ForceFire system
*/

#pragma once

#include <rtype/components.hpp>
#include "../../game/Requests/Requests.hpp"
#include "../../game/Rules.hpp"
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * An attached Force fires with its player: each missile
     * the player fires (Shooting, just before) comes with 1, 2
     * or 3 missiles of the Force, by level, forward from the
     * front, backward from the back.
     */
    class ForceFire {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 51,
            };

        private:
            void* m_Engine;
            Game::Requests* m_Requests;

        public:
            ForceFire() noexcept;

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
