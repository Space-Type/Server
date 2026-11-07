/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Stage1 system
*/

#include "Stage1.hpp"
#include <rtype/game.hpp>
#include <rtype/utils.hpp>
#include <chrono>


RType::Systems::Stage1::Stage1() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
    , m_Random()
{

}

void RType::Systems::Stage1::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& level = engine.get<Protecs::Components::Level>()[entity];
    auto& state = engine.get<Components::LevelState>()[entity];
    float seconds = std::chrono::duration<float>(dt).count();

    if (level.stage != 1 || state.bossSpawned || state.cleared)
        return;

    state.distance += Rules::SCROLL_SPEED * m_Speed * seconds;
    level.progression = static_cast<std::uint32_t>(state.distance);
    if (level.progression >= Rules::STAGE1_LENGTH) {
        level.progression = Rules::STAGE1_LENGTH;
        state.bossSpawned = true;
        spawnBoss();
        return;
    }

    // Every wave the level reached appears on the right (stage-1.waves)
    for (; m_Waves && state.nextWave < m_Waves->size() && (*m_Waves)[state.nextWave].progression <= level.progression; ++state.nextWave) {
        const Game::Wave& wave = (*m_Waves)[state.nextWave];

        m_Requests->spawns.push_back({
            .type = wave.type,
            .position = { Protecs::MAP_MAX_X, wave.y },
            .size = Rules::ENEMY_SIZE,
            .velocity = { -Rules::ENEMY_SPEED, 0.0F },
            .cooldown = m_Random.between(Rules::ENEMY_SHOT_MIN, Rules::ENEMY_SHOT_MAX),
            .health = Rules::PATA_PATA_HEALTH,
            .pattern = wave.pattern,
        });
    }
}

/**
 * At the end of the stage, the Dobkeratops enters from the
 * right and stops there: a body that hurts and absorbs
 * shots, and its core, the only part that takes damage.
 */
void RType::Systems::Stage1::spawnBoss()
{
    constexpr float SCALE = Rules::BOSS_SCALE;
    constexpr float STOP_X = Protecs::MAP_MAX_X - Rules::BOSS_SPRITE.x * SCALE - Rules::BOSS_MARGIN;
    constexpr float Y = (Protecs::MAP_MAX_Y - Rules::BOSS_SPRITE.y * SCALE) / 2.0F;
    constexpr Rules::Vec2D CORE = Rules::BOSS_CORE_OFFSET * SCALE;

    logger.info() << "the stage 1 boss arrives" << std::endl;
    m_Requests->spawns.push_back({
        .type = Rules::EntityCreate::ENEMY_DOBKERATOPS,
        .position = { Protecs::MAP_MAX_X, Y },
        .size = Rules::BOSS_BODY_SIZE * SCALE,
        .velocity = { -Rules::BOSS_SPEED, 0.0F },
        .offset = Rules::BOSS_BODY_OFFSET * SCALE,
        .stopX = STOP_X,
        .scale = SCALE,
        .boss = true,
    });
    m_Requests->spawns.push_back({
        .type = Rules::EntityCreate::ENEMY_DOBKERATOPS_CORE,
        .position = { Protecs::MAP_MAX_X + CORE.x, Y + CORE.y },
        .size = Rules::BOSS_CORE_SIZE * SCALE,
        .velocity = { -Rules::BOSS_SPEED, 0.0F },
        .health = Rules::BOSS_HEALTH,
        .stopX = STOP_X + CORE.x,
    });
}
