/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** PhaseChange system
*/

#pragma once

#include <rtype/components.hpp>
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * A boss with Enrage gets angry once, when its health drops
     * to the threshold: BossPhase 2 (clients show it), bigger
     * bursts, faster swinging parts.
     */
    class PhaseChange {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 118,
            };

        private:
            void* m_Engine;

        public:
            PhaseChange() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Enrage>())
                    .set(Components::Manager::index<Protecs::Components::Health>())
                    .set(Components::Manager::index<Protecs::Components::BossPhase>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
