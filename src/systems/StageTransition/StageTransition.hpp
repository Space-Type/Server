/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** StageTransition system
*/

#pragma once

#include <rtype/components.hpp>
#include "../../game/Rules.hpp"
#include "../../game/Waves/Waves.hpp"
#include <vector>
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * A while after a stage is cleared, the level goes on to
     * the next stage if its file exists; after the last one,
     * the game is won (Level::won, shared with the clients).
     */
    class StageTransition {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 125,
            };

        private:
            void* m_Engine;
            const std::vector<Game::Stage>* m_Stages = nullptr;

        public:
            StageTransition() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Level>())
                    .set(Components::Manager::index<Components::LevelState>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }
            constexpr void setStages(const std::vector<Game::Stage>& stages) noexcept { m_Stages = &stages; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
