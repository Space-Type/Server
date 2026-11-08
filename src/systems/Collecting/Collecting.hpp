/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Collecting system
*/

#pragma once

#include <rtype/components.hpp>
#include "../../game/Requests/Requests.hpp"
#include "../../game/Rules.hpp"
#include <wrecks/ecs.hpp>


namespace RType::Systems {
    /**
     * A player takes a Pickup by touching it, or by shooting
     * it (the item goes to the shooter): a missile stops on
     * it, a beam goes through. A player who is out takes
     * nothing. Runs after Collisions: a shot that already hit
     * an enemy is gone.
     */
    class Collecting {
        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::FIXED_UPDATE,
                .order = 113,
            };

        private:
            void* m_Engine;
            Game::Requests* m_Requests;

        public:
            Collecting() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Transform>())
                    .set(Components::Manager::index<Components::Body>())
                    .set(Components::Manager::index<Components::Pickup>());
            }

            constexpr void setEngine(void* engine) noexcept { m_Engine = engine; }
            constexpr void setRequests(Game::Requests& requests) noexcept { m_Requests = &requests; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);

        private:
            void give(Components::Item item, Wrecks::ECS::Entity player);
            void giveForce(Wrecks::ECS::Entity player);
    };
}
