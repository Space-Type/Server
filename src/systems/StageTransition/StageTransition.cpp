/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** StageTransition system
*/

#include "StageTransition.hpp"
#include <rtype/game.hpp>
#include <rtype/utils.hpp>
#include <chrono>


RType::Systems::StageTransition::StageTransition() noexcept
    : m_Engine(nullptr)
{

}

void RType::Systems::StageTransition::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& level = engine.get<Protecs::Components::Level>()[entity];
    auto& state = engine.get<Components::LevelState>()[entity];

    if (!state.cleared || level.won || !m_Stages)
        return;
    state.clearedFor += std::chrono::duration<float>(dt).count();
    if (state.clearedFor < Rules::STAGE_CLEAR_DELAY)
        return;

    if (level.stage < m_Stages->size()) {
        ++level.stage;
        level.progression = 0;
        state = Components::LevelState{};
        logger.info() << "stage " << static_cast<int>(level.stage) << " starts" << std::endl;
        return;
    }
    level.won = true;
    logger.info() << "victory: every stage is cleared" << std::endl;
}
