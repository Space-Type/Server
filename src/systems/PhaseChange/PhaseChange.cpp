/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** PhaseChange system
*/

#include "PhaseChange.hpp"
#include <rtype/game.hpp>
#include <rtype/utils.hpp>


RType::Systems::PhaseChange::PhaseChange() noexcept
    : m_Engine(nullptr)
{

}

void RType::Systems::PhaseChange::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& enrage = engine.get<Components::Enrage>()[entity];
    const auto& health = engine.get<Protecs::Components::Health>()[entity];

    if (enrage.done || health.hp == 0 || health.hp > enrage.hpAtMost)
        return;

    enrage.done = true;
    engine.get<Protecs::Components::BossPhase>()[entity].phase = 2;
    if (engine.hasComponent<Components::AimedBursts>(entity)) {
        auto& bursts = engine.get<Components::AimedBursts>()[entity];

        bursts.count = enrage.burstCount;
        bursts.burstInterval = enrage.burstInterval;
    }
    for (auto [part, swing] : engine.get<Components::Swing>())
        if (engine.get<Components::Attached>()[part].parent == entity)
            swing.speed *= enrage.swingSpeed;
    logger.info() << "the boss gets angry (" << health.hp << " hp left)" << std::endl;
}
