/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Dobkeratops system
*/

#pragma once

#include <rtype/components.hpp>
#include "../../game/Requests/Requests.hpp"
#include "../../game/Rules.hpp"
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * Stage 1 boss, once in place: bursts of slow shots from
     * its mouth, aimed at the nearest player. When its core
     * dies, the boss and every enemy left go away and the
     * level is cleared (clients show the victory).
     */
    class Dobkeratops {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 45,
            };

        private:
            void* m_Engine;
            Game::Requests* m_Requests;

        public:
            Dobkeratops() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Boss>())
                    .set(Components::Manager::index<Components::Body>())
                    .set(Components::Manager::index<Protecs::Components::Transform>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }
            constexpr void setRequests(Game::Requests& requests) noexcept { m_Requests = &requests; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);

        private:
            [[ nodiscard ]] bool coreAlive() const;
            void clearStage(Wrecks::ECS::Entity boss);
            void shootAtNearestPlayer(Rules::Vec2D from);
    };
}
