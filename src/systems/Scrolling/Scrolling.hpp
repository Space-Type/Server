/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Scrolling system
*/

#pragma once

#include <rtype/components.hpp>
#include "../../game/Rules.hpp"
#include "../../game/Waves/Waves.hpp"
#include <vector>
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * Scrolls the level (Level::progression, in pixels of the
     * stage background) until the length of its stage: then the
     * level stays at its end, for the boss.
     */
    class Scrolling {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 40,
            };

        private:
            void* m_Engine;
            const std::vector<Game::Stage>* m_Stages = nullptr;
            float m_Speed = 1.0F;  // developer option --speed

        public:
            Scrolling() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Level>())
                    .set(Components::Manager::index<Components::LevelState>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }
            constexpr void setStages(const std::vector<Game::Stage>& stages) noexcept { m_Stages = &stages; }
            constexpr void setSpeed(float speed) noexcept { m_Speed = speed; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
