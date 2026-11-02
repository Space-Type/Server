/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** BufferedInput system
*/

#pragma once

#include <rtype/components.hpp>
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    class BufferedInput {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 0
            };

        private:
            void* m_Engine;
            std::uint64_t* m_SimulationTick;

        public:
            BufferedInput() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::InputBuffer>())
                    .set(Components::Manager::index<Protecs::Components::PlayerInput>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }
            constexpr void setSimulationTick(std::uint64_t& tick) noexcept { m_SimulationTick = &tick; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime);
    };
}
