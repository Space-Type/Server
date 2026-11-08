/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Looting system
*/

#pragma once

#include <rtype/components.hpp>
#include "../../game/Requests/Requests.hpp"
#include "../../game/Rules.hpp"
#include <optional>
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * An enemy killed by a player may drop its Loot: a Pickup
     * appears where it was, drifting with the background.
     * Runs after Collisions, which records the kills.
     */
    class Looting {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 111,
            };

        private:
            void* m_Engine;
            Game::Requests* m_Requests;
            Rules::Random m_Random;
            std::optional<float> m_Chance;  // developer option --drop-rate: the chance of every Loot

        public:
            Looting() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Transform>())
                    .set(Components::Manager::index<Components::Body>())
                    .set(Components::Manager::index<Components::Loot>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }
            constexpr void setRequests(Game::Requests& requests) noexcept { m_Requests = &requests; }
            inline void seed(std::uint32_t seed) noexcept { m_Random.seed(seed); }
            inline void setChance(std::optional<float> chance) noexcept { m_Chance = chance; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
