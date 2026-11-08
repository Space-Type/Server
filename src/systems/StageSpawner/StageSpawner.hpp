/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** StageSpawner system
*/

#pragma once

#include <rtype/components.hpp>
#include "../../game/Requests/Requests.hpp"
#include "../../game/Rules.hpp"
#include "../../game/Waves/Waves.hpp"
#include <vector>
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * Makes the enemies of the stage file appear on the right as
     * the level reaches them, then its boss at the end of the
     * stage (a stage without boss is cleared there).
     */
    class StageSpawner {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 41,
            };

        private:
            void* m_Engine;
            Game::Requests* m_Requests;
            Rules::Random m_Random;
            const std::vector<Game::Stage>* m_Stages = nullptr;

        public:
            StageSpawner() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Level>())
                    .set(Components::Manager::index<Components::LevelState>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }
            constexpr void setRequests(Game::Requests& requests) noexcept { m_Requests = &requests; }
            constexpr void setStages(const std::vector<Game::Stage>& stages) noexcept { m_Stages = &stages; }
            inline void seed(std::uint32_t seed) noexcept { m_Random.seed(seed); }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
