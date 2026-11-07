/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Stage1 system
*/

#pragma once

#include <rtype/components.hpp>
#include "../../game/Requests/Requests.hpp"
#include "../../game/Rules.hpp"
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * First stage: scrolls the level (Level::progression, in
     * pixels of stage-1.png) and makes Bydos appear on the right
     * at random heights. At the end of the stage, the level
     * goes on to stage 2.
     */
    class Stage1 {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 40,
            };

        private:
            void* m_Engine;
            Game::Requests* m_Requests;
            Rules::Random m_Random;

        public:
            Stage1() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Level>())
                    .set(Components::Manager::index<Components::LevelState>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }
            constexpr void setRequests(Game::Requests& requests) noexcept { m_Requests = &requests; }
            inline void seed(std::uint32_t seed) noexcept { m_Random.seed(seed); }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
