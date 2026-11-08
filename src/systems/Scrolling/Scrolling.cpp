/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Scrolling system
*/

#include "Scrolling.hpp"
#include <rtype/game.hpp>
#include <chrono>


RType::Systems::Scrolling::Scrolling() noexcept
    : m_Engine(nullptr)
{

}

void RType::Systems::Scrolling::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& level = engine.get<Protecs::Components::Level>()[entity];
    auto& state = engine.get<Components::LevelState>()[entity];

    if (!m_Stages || level.stage == 0 || level.stage > m_Stages->size())
        return;

    std::uint32_t length = (*m_Stages)[level.stage - 1].length;

    if (state.cleared || (length > 0 && level.progression >= length))
        return;
    state.distance += Rules::SCROLL_SPEED * m_Speed * std::chrono::duration<float>(dt).count();
    level.progression = static_cast<std::uint32_t>(state.distance);
    if (length > 0 && level.progression > length)
        level.progression = length;
}
