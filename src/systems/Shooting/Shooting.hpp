/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Shooting system
*/

#pragma once

#include <rtype/components.hpp>
#include "../../game/Requests/Requests.hpp"
#include "../../game/Rules.hpp"
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * Counts down the shot cooldowns and invulnerability.
     * Players fire a missile when pressing the trigger and a
     * charged beam when releasing it (unless they are out);
     * enemies shoot at random intervals. Shots leave from the
     * front.
     */
    class Shooting {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 50,
            };

        private:
            void* m_Engine;
            Game::Requests* m_Requests;
            Rules::Random m_Random;

        public:
            Shooting() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Transform>())
                    .set(Components::Manager::index<Components::Body>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }
            constexpr void setRequests(Game::Requests& requests) noexcept { m_Requests = &requests; }
            inline void seed(std::uint32_t seed) noexcept { m_Random.seed(seed); }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);

        private:
            void shootAsPlayer(Wrecks::ECS::Entity entity, Components::Body& body, float seconds);
            void shootAsEnemy(Wrecks::ECS::Entity entity, Components::Body& body);
    };
}
