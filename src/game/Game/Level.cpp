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
#include <type_traits>
#include <variant>


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

/**
 * Creates the entity of a request, then its parts: a part
 * is placed from its parent's position and Attached to it.
 */
void RType::Game::Game::spawnBody(const SpawnRequest& request, std::optional<Wrecks::ECS::Entity> parent)
{
    auto entity = m_Engine.createEntity();
    auto networkId = Components::NetworkID::fromEntity(entity);
    Protecs::Utils::Vec2D<float> position = request.position;

    if (parent)
        position += m_Engine.get<Protecs::Components::Transform>()[*parent].position;
    m_Engine.addComponent(entity, Components::NetworkID{ networkId });
    m_Engine.addComponent(entity, Components::Body{
        .type = request.type, .size = request.size, .offset = request.offset, .velocity = request.velocity,
        .owner = request.owner, .cooldown = request.cooldown, .damage = request.damage,
        .pattern = request.pattern, .baseY = position.y,
    });
    if (parent)
        m_Engine.addComponent(entity, Components::Attached{ .parent = *parent, .offset = request.position });
    broadcastPacket(Packets::EntityCreate{ .simulationTick = m_SimulationTick, .id = networkId.id, .type = request.type });
    addSharedComponent(entity, Protecs::Components::Transform{ .position = position, .rotation = 0.0F, .scale = request.scale });
    for (const auto& component : request.components) {
        std::visit([&](const auto& value) {
            using Component = std::decay_t<decltype(value)>;

            // Shared components go to the clients too
            if constexpr (std::is_same_v<Component, Protecs::Components::Health> || std::is_same_v<Component, Protecs::Components::BossPhase>)
                addSharedComponent(entity, Component{ value });
            else
                m_Engine.addComponent(entity, Component{ value });
        }, component);
    }
    for (const auto& part : request.parts)
        spawnBody(part, entity);
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

/**
 * The level starts with the first player: its entity holds
 * the stage and its progression (shared with the clients),
 * and the stage systems (Scrolling, StageSpawner) run on it.
 */
void RType::Game::Game::startLevel()
{
    auto level = m_Engine.createEntity();
    auto networkId = Components::NetworkID::fromEntity(level);

    m_Level = level;
    m_Engine.addComponent(level, Components::NetworkID{ networkId });
    // --start: the waves before the starting progression are skipped, not all spawned at once
    std::size_t firstWave = 0;

    if (!m_Stages.empty())
        firstWave = static_cast<std::size_t>(std::ranges::lower_bound(m_Stages[0].waves, m_Dev.start, {}, &Wave::progression) - m_Stages[0].waves.begin());
    m_Engine.addComponent(level, Components::LevelState{ .distance = static_cast<float>(m_Dev.start), .nextWave = firstWave });
    broadcastPacket(Packets::EntityCreate{ .simulationTick = m_SimulationTick, .id = networkId.id, .type = Packets::EntityCreate::GENERIC });
    addSharedComponent(level, Protecs::Components::Level{ .progression = m_Dev.start, .stage = 1 });
    logger.info() << "level started" << std::endl;
}

/**
 * With no player left, the level and everything still flying
 * are deleted: the next player starts the level over.
 */
void RType::Game::Game::endLevel()
{
    std::vector<Wrecks::ECS::Entity> leftovers;

    for (auto [entity, body] : m_Engine.get<Components::Body>())
        leftovers.push_back(entity);
    if (m_Level)
        leftovers.push_back(*m_Level);
    for (auto entity : leftovers) {
        broadcastPacket(Packets::EntityDelete{ .id = m_Engine.get<Components::NetworkID>()[entity].id });
        m_Engine.deleteEntity(entity);
    }
    m_Requests.spawns.clear();
    m_Requests.destroys.clear();
    m_Level.reset();
    logger.info() << "level ended (no player left)" << std::endl;
}

void RType::Game::Game::setDevOptions(const DevOptions& dev)
{
    m_Dev = dev;
    m_Engine.get<Systems::Collisions>().setInvincible(dev.god);
    m_Engine.get<Systems::Scrolling>().setSpeed(dev.speed);
    if (dev.any())
        logger.warning() << "developer options on: god " << (dev.god ? "on" : "off") << ", speed x" << dev.speed
                         << ", start at " << dev.start << " (never on a real server)" << std::endl;
}
