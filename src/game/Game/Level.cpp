/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Game class: level lifecycle and the systems' entity requests
*/

#include "Game.hpp"
#include "../Rules.hpp"
#include <rtype/game.hpp>
#include <rtype/utils.hpp>
#include <algorithm>
#include <array>


namespace Packets = Protecs::Networking::Packets;


/**
 * Applies what the systems asked during the last fixed
 * update: deletions first (a destroyed shooter does not
 * matter to its missiles), then creations. Clients are
 * told about every change.
 */
void RType::Game::Game::applyRequests()
{
    for (auto entity : m_Requests.destroys) {
        if (!m_Engine.hasComponent<Components::NetworkID>(entity))
            continue;
        broadcastPacket(Packets::EntityDelete{ .id = m_Engine.get<Components::NetworkID>()[entity].id });
        m_Engine.deleteEntity(entity);
    }
    for (const auto& request : m_Requests.spawns)
        spawnBody(request);
    m_Requests.destroys.clear();
    m_Requests.spawns.clear();
}

void RType::Game::Game::spawnBody(const SpawnRequest& request)
{
    auto entity = m_Engine.createEntity();
    auto networkId = Components::NetworkID::fromEntity(entity);

    m_Engine.addComponent(entity, Components::NetworkID{ networkId });
    m_Engine.addComponent(entity, Components::Body{
        .type = request.type, .size = request.size, .velocity = request.velocity,
        .owner = request.owner, .cooldown = request.cooldown,
    });
    broadcastPacket(Packets::EntityCreate{ .simulationTick = m_SimulationTick, .id = networkId.id, .type = request.type });
    addSharedComponent(entity, Protecs::Components::Transform{ .position = request.position, .rotation = 0.0F, .scale = 1.0F });
}

/**
 * Gameplay data of a new player: its hitbox, and its
 * stats with the lowest free slot (its colour on clients).
 */
void RType::Game::Game::initPlayerBody(Wrecks::ECS::Entity player)
{
    std::array<bool, 4> taken = {};

    for (auto [entity, stats] : m_Engine.get<Protecs::Components::PlayerStats>())
        if (stats.slot < taken.size())
            taken[stats.slot] = true;

    auto slot = static_cast<std::uint8_t>(std::find(taken.begin(), taken.end(), false) - taken.begin());

    m_Engine.addComponent(player, Components::Body{ .type = Packets::EntityCreate::PLAYER, .size = Rules::PLAYER_SIZE });
    m_Engine.get<Protecs::Components::Transform>()[player].position = Rules::respawnPosition(slot);
    addSharedComponent(player, Protecs::Components::PlayerStats{ .slot = slot, .lives = Rules::PLAYER_LIVES, .score = 0 });
}
