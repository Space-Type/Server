/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Game class: gameplay (enemies, shots, collisions)
*/

#include "Game.hpp"
#include <rtype/game.hpp>
#include <rtype/utils.hpp>
#include <algorithm>
#include <array>
#include <chrono>
#include <random>


namespace {
    namespace Packets = Protecs::Networking::Packets;
    using EntityType = Packets::EntityCreate::EntityType;
    using Vec2D = Protecs::Utils::Vec2D<float>;

    // Sizes match the client sprites (world units, drawn 1:1)
    constexpr Vec2D PLAYER_SIZE = { Protecs::PLAYER_WIDTH, Protecs::PLAYER_HEIGHT };
    constexpr Vec2D ENEMY_SIZE = { 33.0F, 36.0F };
    constexpr Vec2D PLAYER_MISSILE_SIZE = { 33.0F, 10.0F };
    constexpr Vec2D ENEMY_MISSILE_SIZE = { 9.0F, 8.0F };

    constexpr float PLAYER_SHOT_COOLDOWN = 0.25F;
    constexpr float PLAYER_MISSILE_SPEED = 300.0F;
    constexpr float ENEMY_SPEED = 60.0F;
    constexpr float ENEMY_MISSILE_SPEED = 150.0F;
    constexpr float ENEMY_SPAWN_MIN = 1.0F;  // seconds between two enemies
    constexpr float ENEMY_SPAWN_MAX = 2.0F;
    constexpr float ENEMY_SHOT_MIN = 1.5F;   // seconds between two shots of an enemy
    constexpr float ENEMY_SHOT_MAX = 3.5F;

    constexpr std::uint8_t PLAYER_LIVES = 3;
    constexpr std::uint32_t ENEMY_SCORE = 100;
    constexpr float RESPAWN_INVULNERABILITY = 2.0F;
    constexpr float OFFSCREEN_MARGIN = 64.0F;  // beyond it, an enemy or a missile is gone

    bool isEnemy(EntityType type) noexcept
    {
        return type == Packets::EntityCreate::ENEMY_PATA_PATA || type == Packets::EntityCreate::ENEMY_SCANT;
    }

    bool overlap(Vec2D a, Vec2D aSize, Vec2D b, Vec2D bSize) noexcept
    {
        return a.x < b.x + bSize.x && b.x < a.x + aSize.x && a.y < b.y + bSize.y && b.y < a.y + aSize.y;
    }

    Vec2D respawnPosition(std::uint8_t slot) noexcept
    {
        return { 40.0F, 60.0F + 90.0F * static_cast<float>(slot) };
    }
}


/**
 * Runs the gameplay at a fixed rate (GAMEPLAY_RATE),
 * whatever the speed of the server loop: every step
 * moves the same amount of time.
 */
void RType::Game::Game::updateGameplay(Wrecks::DeltaTime dt)
{
    m_GameplayElapsedTime += dt;

    while (m_GameplayElapsedTime >= GAMEPLAY_RATE) {
        stepGameplay(std::chrono::duration<float>(GAMEPLAY_RATE).count());
        m_GameplayElapsedTime -= GAMEPLAY_RATE;
    }
}

void RType::Game::Game::stepGameplay(float seconds)
{
    spawnEnemies(seconds);
    shoot(seconds);
    moveBodies(seconds);
    collide();
    destroyQueued();
}

/**
 * Gameplay data of a new player: its hitbox, and its
 * stats with the lowest free slot (its colour on clients).
 */
void RType::Game::Game::initPlayerGameplay(Wrecks::ECS::Entity player)
{
    std::array<bool, 4> taken = {};

    for (auto [entity, stats] : m_Engine.get<Protecs::Components::PlayerStats>())
        if (stats.slot < taken.size())
            taken[stats.slot] = true;

    auto slot = static_cast<std::uint8_t>(std::find(taken.begin(), taken.end(), false) - taken.begin());

    m_Engine.addComponent(player, Components::Body{ .type = Packets::EntityCreate::PLAYER, .size = PLAYER_SIZE });
    m_Engine.get<Protecs::Components::Transform>()[player].position = respawnPosition(slot);
    addSharedComponent(player, Protecs::Components::PlayerStats{ .slot = slot, .lives = PLAYER_LIVES, .score = 0 });
}

/**
 * Creates an enemy or a missile and tells every client.
 */
Wrecks::ECS::Entity RType::Game::Game::spawn(EntityType type, Vec2D position, Vec2D size, Vec2D velocity, Wrecks::ECS::Entity owner)
{
    auto entity = m_Engine.createEntity();
    auto networkId = Components::NetworkID::fromEntity(entity);

    m_Engine.addComponent(entity, Components::NetworkID{ networkId });
    m_Engine.addComponent(entity, Components::Body{ .type = type, .size = size, .velocity = velocity, .owner = owner });
    broadcastPacket(Packets::EntityCreate{ .simulationTick = m_SimulationTick, .id = networkId.id, .type = type });
    addSharedComponent(entity, Protecs::Components::Transform{ .position = position, .rotation = 0.0F, .scale = 1.0F });
    return entity;
}

void RType::Game::Game::spawnEnemies(float seconds)
{
    m_EnemySpawnTimer -= seconds;
    if (m_EnemySpawnTimer > 0.0F || m_Players.empty())
        return;

    std::uniform_real_distribution<float> delay(ENEMY_SPAWN_MIN, ENEMY_SPAWN_MAX);
    std::uniform_real_distribution<float> height(0.0F, Protecs::MAP_MAX_Y - ENEMY_SIZE.y);
    std::uniform_real_distribution<float> firstShot(ENEMY_SHOT_MIN, ENEMY_SHOT_MAX);

    m_EnemySpawnTimer = delay(m_Random);

    auto enemy = spawn(Packets::EntityCreate::ENEMY_PATA_PATA, { Protecs::MAP_MAX_X, height(m_Random) }, ENEMY_SIZE, { -ENEMY_SPEED, 0.0F });

    m_Engine.get<Components::Body>()[enemy].cooldown = firstShot(m_Random);
}

/**
 * Players shoot while their trigger is held, enemies
 * at random intervals. Shots leave from the front.
 */
void RType::Game::Game::shoot(float seconds)
{
    struct Shot { EntityType type; Vec2D position; Vec2D size; Vec2D velocity; Wrecks::ECS::Entity owner; };
    std::vector<Shot> shots;
    std::uniform_real_distribution<float> enemyDelay(ENEMY_SHOT_MIN, ENEMY_SHOT_MAX);

    for (auto [entity, unused] : m_Engine.get<Components::Body>()) {
        auto& body = m_Engine.get<Components::Body>()[entity];
        const auto& position = m_Engine.get<Protecs::Components::Transform>()[entity].position;

        body.cooldown = std::max(0.0F, body.cooldown - seconds);
        body.invulnerable = std::max(0.0F, body.invulnerable - seconds);
        if (body.cooldown > 0.0F)
            continue;

        if (body.type == Packets::EntityCreate::PLAYER && m_Engine.get<Protecs::Components::PlayerInput>()[entity].trigger) {
            body.cooldown = PLAYER_SHOT_COOLDOWN;
            shots.push_back({ Packets::EntityCreate::PLAYER_MISSILE,
                              { position.x + body.size.x, position.y + (body.size.y - PLAYER_MISSILE_SIZE.y) / 2.0F },
                              PLAYER_MISSILE_SIZE, { PLAYER_MISSILE_SPEED, 0.0F }, entity });
        } else if (isEnemy(body.type)) {
            body.cooldown = enemyDelay(m_Random);
            shots.push_back({ Packets::EntityCreate::ENEMY_MISSILE,
                              { position.x - ENEMY_MISSILE_SIZE.x, position.y + (body.size.y - ENEMY_MISSILE_SIZE.y) / 2.0F },
                              ENEMY_MISSILE_SIZE, { -ENEMY_MISSILE_SPEED, 0.0F }, entity });
        }
    }

    // Created after the loop: never while iterating over the bodies
    for (const Shot& shot : shots)
        spawn(shot.type, shot.position, shot.size, shot.velocity, shot.owner);
}

/**
 * Enemies and missiles fly straight, and are gone
 * once they left the screen. Players move with the
 * shared systems (PlayerMovement, Motion, PlayerBounds).
 */
void RType::Game::Game::moveBodies(float seconds)
{
    for (auto [entity, unused] : m_Engine.get<Components::Body>()) {
        const auto& body = m_Engine.get<Components::Body>()[entity];

        if (body.type == Packets::EntityCreate::PLAYER)
            continue;

        auto& position = m_Engine.get<Protecs::Components::Transform>()[entity].position;

        position += body.velocity * seconds;
        if (position.x < -OFFSCREEN_MARGIN || position.x > Protecs::MAP_MAX_X + OFFSCREEN_MARGIN)
            destroyLater(entity);
    }
}

/**
 * Player missiles destroy enemies (score to the shooter),
 * enemies and their missiles hit players.
 */
void RType::Game::Game::collide()
{
    std::vector<Wrecks::ECS::Entity> bodies;

    for (auto [entity, unused] : m_Engine.get<Components::Body>())
        bodies.push_back(entity);

    for (auto first : bodies) {
        for (auto second : bodies) {
            if (first == second || std::ranges::find(m_Destroyed, first) != m_Destroyed.end()
                || std::ranges::find(m_Destroyed, second) != m_Destroyed.end())
                continue;

            const auto& a = m_Engine.get<Components::Body>()[first];
            const auto& b = m_Engine.get<Components::Body>()[second];

            if (!overlap(m_Engine.get<Protecs::Components::Transform>()[first].position, a.size,
                         m_Engine.get<Protecs::Components::Transform>()[second].position, b.size))
                continue;

            if (a.type == Packets::EntityCreate::PLAYER_MISSILE && isEnemy(b.type)) {
                if (m_Engine.hasComponent<Protecs::Components::PlayerStats>(a.owner)) {
                    m_Engine.get<Protecs::Components::PlayerStats>()[a.owner].score += ENEMY_SCORE;
                    sendSharedComponent<Protecs::Components::PlayerStats>(a.owner);
                }
                destroyLater(first);
                destroyLater(second);
            } else if (a.type == Packets::EntityCreate::PLAYER && (isEnemy(b.type) || b.type == Packets::EntityCreate::ENEMY_MISSILE)) {
                if (a.invulnerable > 0.0F)
                    continue;
                destroyLater(second);
                hitPlayer(first);
            }
        }
    }
}

/**
 * A hit player loses a life and comes back at its
 * start, invulnerable for a moment. With no life
 * left, it starts over (game over is up to clients).
 */
void RType::Game::Game::hitPlayer(Wrecks::ECS::Entity player)
{
    auto& stats = m_Engine.get<Protecs::Components::PlayerStats>()[player];
    auto& body = m_Engine.get<Components::Body>()[player];

    stats.lives = stats.lives > 0 ? stats.lives - 1 : 0;
    if (stats.lives == 0) {
        logger.info() << "player \"" << m_Engine.get<Components::Player>()[player].name << "\" lost all lives ("
                      << stats.score << " points)" << std::endl;
        stats.lives = PLAYER_LIVES;
        stats.score = 0;
    }
    body.invulnerable = RESPAWN_INVULNERABILITY;
    m_Engine.get<Protecs::Components::Transform>()[player].position = respawnPosition(stats.slot);
    m_Engine.get<Protecs::Components::Velocity>()[player] = {};
    sendSharedComponent<Protecs::Components::PlayerStats>(player);
    sendSharedComponent<Protecs::Components::Transform>(player);
}

void RType::Game::Game::destroyLater(Wrecks::ECS::Entity entity)
{
    if (std::ranges::find(m_Destroyed, entity) == m_Destroyed.end())
        m_Destroyed.push_back(entity);
}

void RType::Game::Game::destroyQueued()
{
    for (auto entity : m_Destroyed) {
        broadcastPacket(Packets::EntityDelete{ .id = m_Engine.get<Components::NetworkID>()[entity].id });
        m_Engine.deleteEntity(entity);
    }
    m_Destroyed.clear();
}
