/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Swinging system
*/

#pragma once

#include <rtype/components.hpp>
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * Moves the swinging parts (a boss tail): each one sets its
     * place around its parent (Attached::offset), which the
     * Attachment system then applies.
     */
    class Swinging {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 112,
            };

        private:
            void* m_Engine;

        public:
            Swinging() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Swing>())
                    .set(Components::Manager::index<Components::Attached>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
