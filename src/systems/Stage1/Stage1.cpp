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

    if (level.stage != 1)
        return;

    state.distance += Rules::SCROLL_SPEED * seconds;
    level.progression = static_cast<std::uint32_t>(state.distance);
    if (level.progression >= Rules::STAGE1_LENGTH) {
        logger.info() << "stage 1 cleared, going on to stage 2" << std::endl;
        level = { .progression = 0, .stage = 2 };
        state = {};
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
