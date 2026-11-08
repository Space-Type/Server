/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** BossDefeat system
*/

#include "BossDefeat.hpp"
#include <rtype/game.hpp>
#include <rtype/utils.hpp>


RType::Systems::BossDefeat::BossDefeat() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

void RType::Systems::BossDefeat::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    if (!m_Requests->destroying(entity) || engine.get<Protecs::Components::Health>()[entity].hp > 0)
        return;

    for (auto [other, body] : engine.get<Components::Body>())
        if (engine.hasComponent<Components::Enemy>(other) || body.type == Rules::EntityCreate::ENEMY_MISSILE)
            m_Requests->destroy(other);
    for (auto [level, state] : engine.get<Components::LevelState>()) {
        state.cleared = true;
        logger.info() << "stage " << static_cast<int>(engine.get<Protecs::Components::Level>()[level].stage) << " cleared" << std::endl;
    }
}
