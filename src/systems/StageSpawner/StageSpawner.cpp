/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** StageSpawner system
*/

#include "StageSpawner.hpp"
#include <rtype/game.hpp>
#include "../../game/Blueprints/Blueprints.hpp"
#include <rtype/utils.hpp>


RType::Systems::StageSpawner::StageSpawner() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
    , m_Random()
{

}

void RType::Systems::StageSpawner::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& level = engine.get<Protecs::Components::Level>()[entity];
    auto& state = engine.get<Components::LevelState>()[entity];

    if (!m_Stages || level.stage == 0 || level.stage > m_Stages->size() || state.bossSpawned || state.cleared)
        return;

    const Game::Stage& stage = (*m_Stages)[level.stage - 1];

    // Every wave the level reached appears on the right
    for (; state.nextWave < stage.waves.size() && stage.waves[state.nextWave].progression <= level.progression; ++state.nextWave) {
        Game::Wave wave = stage.waves[state.nextWave];

        if (wave.randomY)
            wave.y = m_Random.between(Rules::RANDOM_Y_MIN, Rules::RANDOM_Y_MAX);
        m_Requests->spawns.push_back(Game::Blueprints::enemy(wave, m_Random.between(Rules::ENEMY_SHOT_MIN, Rules::ENEMY_SHOT_MAX)));
    }

    if (stage.length == 0 || level.progression < stage.length)
        return;
    state.bossSpawned = true;
    if (auto boss = Game::Blueprints::boss(stage.boss)) {
        logger.info() << "stage " << static_cast<int>(level.stage) << ": " << stage.boss << " arrives" << std::endl;
        m_Requests->spawns.push_back(std::move(*boss));
    } else {
        state.cleared = true;
        logger.info() << "stage " << static_cast<int>(level.stage) << " cleared (no boss)" << std::endl;
    }
}
