/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** ForceFire system
*/

#include "ForceFire.hpp"
#include <rtype/game.hpp>
#include <array>
#include <cstddef>


RType::Systems::ForceFire::ForceFire() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

void RType::Systems::ForceFire::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& force = engine.get<Components::Force>()[entity];
    const auto& body = engine.get<Components::Body>()[entity];
    const auto& position = engine.get<Protecs::Components::Transform>()[entity].position;

    if (force.state != Components::Force::State::Attached || m_Requests->destroying(entity))
        return;

    // Vertical speeds of its missiles, by level
    static constexpr std::array<std::array<float, 3>, Rules::FORCE_LEVELS> SPREADS = {{
        { 0.0F, 0.0F, 0.0F },
        { -Rules::FORCE_SPREAD_SPEED, Rules::FORCE_SPREAD_SPEED, 0.0F },
        { 0.0F, -Rules::FORCE_SPREAD_SPEED, Rules::FORCE_SPREAD_SPEED },
    }};
    bool front = force.side == Components::Force::Side::Front;
    std::size_t level = force.level >= 1 && force.level <= Rules::FORCE_LEVELS ? force.level : 1;
    std::size_t fired = 0;

    // Only the missiles the player fired this step: the Force's own come after them
    for (std::size_t i = 0, count = m_Requests->spawns.size(); i < count; ++i)
        fired += m_Requests->spawns[i].type == Rules::EntityCreate::PLAYER_MISSILE && m_Requests->spawns[i].owner == body.owner;

    for (std::size_t shot = 0; shot < fired; ++shot) {
        for (std::size_t i = 0; i < level; ++i) {
            m_Requests->spawns.push_back({
                .type = Rules::EntityCreate::PLAYER_MISSILE,
                .position = {
                    front ? position.x + body.offset.x + body.size.x : position.x + body.offset.x - Rules::PLAYER_MISSILE_SIZE.x,
                    position.y + body.offset.y + (body.size.y - Rules::PLAYER_MISSILE_SIZE.y) / 2.0F,
                },
                .size = Rules::PLAYER_MISSILE_SIZE,
                .velocity = { front ? Rules::PLAYER_MISSILE_SPEED : -Rules::PLAYER_MISSILE_SPEED, SPREADS[level - 1][i] },
                .owner = body.owner,
                .damage = Rules::MISSILE_DAMAGE,
            });
        }
    }
}
