/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** enemy and boss blueprints
*/

#include "Blueprints.hpp"
#include "../Rules.hpp"
#include <array>
#include <utility>


namespace {
    using RType::Rules::Vec2D;
    using RType::Rules::EntityCreate;
    namespace Components = RType::Components;

    /**
     * Stage 1 boss: the Dobkeratops, without its tail. Its
     * core (the creature in its chest) is the weak point and
     * leads; its body is an armored part around it, which
     * absorbs shots. Positions in pixels of its sprite
     * (r-typesheet30, 155 x 204), drawn SCALE times bigger.
     */
    RType::Game::SpawnRequest dobkeratops()
    {
        constexpr float SCALE = 1.6F;                         // ~70 % of the screen height
        constexpr Vec2D SPRITE = { 155.0F, 204.0F };
        constexpr Vec2D BODY_OFFSET = { 95.0F, 40.0F };       // solid part: hurts on contact, absorbs shots
        constexpr Vec2D BODY_SIZE = { 60.0F, 160.0F };
        constexpr Vec2D CORE_OFFSET = { 72.0F, 95.0F };       // the weak point
        constexpr Vec2D CORE_SIZE = { 28.0F, 30.0F };
        constexpr Vec2D MOUTH = { 5.0F, 55.0F };              // where its shots leave
        constexpr float SPEED = 40.0F;                        // entering from the right
        constexpr float MARGIN = 10.0F;                       // from the right edge once in place
        constexpr std::uint16_t HEALTH = 60;
        constexpr std::uint32_t REWARD = 5000;

        constexpr Vec2D CORE = CORE_OFFSET * SCALE;
        constexpr float TOP = (Protecs::MAP_MAX_Y - SPRITE.y * SCALE) / 2.0F;
        constexpr float STOP_X = Protecs::MAP_MAX_X - SPRITE.x * SCALE - MARGIN;

        return {
            .type = EntityCreate::ENEMY_DOBKERATOPS_CORE,
            .position = { Protecs::MAP_MAX_X + CORE.x, TOP + CORE.y },
            .size = CORE_SIZE * SCALE,
            .velocity = { -SPEED, 0.0F },
            .components = {
                Protecs::Components::Health{ .hp = HEALTH, .max = HEALTH },
                Components::Enemy{ .reward = REWARD },
                Components::Boss{},
                Components::StopAt{ .x = STOP_X + CORE.x },
                Components::AimedBursts{ .count = 3, .shotInterval = 0.25F, .burstInterval = 2.0F, .speed = 120.0F,
                                         .muzzle = (MOUTH - CORE_OFFSET) * SCALE },
            },
            .parts = {{
                .type = EntityCreate::ENEMY_DOBKERATOPS,
                .position = { -CORE.x, -CORE.y },
                .size = BODY_SIZE * SCALE,
                .offset = BODY_OFFSET * SCALE,
                .scale = SCALE,
                .components = { Components::Enemy{}, Components::Armored{} },
            }},
        };
    }

    using BossBlueprint = RType::Game::SpawnRequest (*)();

    constexpr std::array<std::pair<std::string_view, BossBlueprint>, 1> BOSSES = {{
        { "dobkeratops", &dobkeratops },
    }};
}


auto RType::Game::Blueprints::enemy(const Wave& wave, float cooldown) -> SpawnRequest
{
    return {
        .type = wave.type,
        .position = { Protecs::MAP_MAX_X, wave.y },
        .size = Rules::ENEMY_SIZE,
        .velocity = { -Rules::ENEMY_SPEED, 0.0F },
        .cooldown = cooldown,
        .pattern = wave.pattern,
        .components = {
            Protecs::Components::Health{ .hp = Rules::PATA_PATA_HEALTH, .max = Rules::PATA_PATA_HEALTH },
            Components::Enemy{ .reward = Rules::ENEMY_SCORE },
            Components::RandomShots{ .min = Rules::ENEMY_SHOT_MIN, .max = Rules::ENEMY_SHOT_MAX },
        },
    };
}

bool RType::Game::Blueprints::isBoss(std::string_view name) noexcept
{
    for (const auto& [known, blueprint] : BOSSES)
        if (known == name)
            return true;
    return false;
}

auto RType::Game::Blueprints::boss(std::string_view name) -> std::optional<SpawnRequest>
{
    for (const auto& [known, blueprint] : BOSSES)
        if (known == name)
            return blueprint();
    return std::nullopt;
}
