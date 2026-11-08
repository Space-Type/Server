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
#include <cmath>
#include <vector>
#include <utility>


namespace {
    using RType::Rules::Vec2D;
    using RType::Rules::EntityCreate;
    namespace Components = RType::Components;

    /**
     * Stage 1 boss: the Dobkeratops. Its core (the creature
     * in its chest) is the weak point and leads; its body is
     * an armored part around it, which absorbs shots, and its
     * tail a chain of armored segments sweeping the screen.
     * At a third of its health it gets angry (phase 2): bigger
     * bursts, faster tail. Positions in pixels of its sprite
     * (r-typesheet30, 155 x 204), drawn SCALE times bigger.
     */
    namespace Dobkeratops {
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

        // Tail: segments of r-typesheet30a (34 x 34), from under the body, sweeping to the left
        constexpr Vec2D TAIL_ROOT = { 40.0F, 175.0F };        // in the sprite
        constexpr int TAIL_SEGMENTS = 10;
        constexpr float SEGMENT_SPACING = 26.0F;              // world units between two segments
        constexpr Vec2D SEGMENT_SIZE = { 34.0F, 34.0F };
        constexpr Vec2D SEGMENT_HITBOX = { 26.0F, 26.0F };   // a little smaller than the ball
        constexpr float TAIL_ANGLE = 3.45F;                   // radians: to the left, a little upwards
        constexpr float TAIL_SWEEP = 0.8F;                    // radians on each side
        constexpr float TAIL_PERIOD = 4.0F;                   // seconds per sweep
        constexpr float TAIL_LAG = 0.3F;                      // radians between two segments: the whip

        // Phase 2, at a third of its health
        constexpr Components::Enrage ENRAGE = { .hpAtMost = HEALTH / 3, .burstCount = 5, .burstInterval = 1.5F, .swingSpeed = 1.5F };

        constexpr Vec2D CORE = CORE_OFFSET * SCALE;

        // Its tail, from the core: the top-left corner of the first segment
        std::vector<RType::Game::SpawnRequest> tail()
        {
            constexpr Vec2D ROOT = (TAIL_ROOT - CORE_OFFSET) * SCALE - SEGMENT_SIZE / 2.0F;
            std::vector<RType::Game::SpawnRequest> segments;

            for (int i = 1; i <= TAIL_SEGMENTS; ++i) {
                float length = SEGMENT_SPACING * static_cast<float>(i);

                segments.push_back({
                    .type = EntityCreate::ENEMY_DOBKERATOPS_TAIL,
                    .position = ROOT + Vec2D{ std::cos(TAIL_ANGLE), std::sin(TAIL_ANGLE) } * length,
                    .size = SEGMENT_HITBOX,
                    .offset = (SEGMENT_SIZE - SEGMENT_HITBOX) / 2.0F,
                    .components = {
                        Components::Enemy{},
                        Components::Armored{},
                        Components::Swing{ .anchor = ROOT, .length = length, .baseAngle = TAIL_ANGLE, .amplitude = TAIL_SWEEP,
                                           .period = TAIL_PERIOD, .lag = TAIL_LAG * static_cast<float>(i) },
                    },
                });
            }
            return segments;
        }

        RType::Game::SpawnRequest blueprint()
        {
            constexpr float TOP = (Protecs::MAP_MAX_Y - SPRITE.y * SCALE) / 2.0F;
            constexpr float STOP_X = Protecs::MAP_MAX_X - SPRITE.x * SCALE - MARGIN;

            std::vector<RType::Game::SpawnRequest> parts = {{
                .type = EntityCreate::ENEMY_DOBKERATOPS,
                .position = { -CORE.x, -CORE.y },
                .size = BODY_SIZE * SCALE,
                .offset = BODY_OFFSET * SCALE,
                .scale = SCALE,
                .components = { Components::Enemy{}, Components::Armored{} },
            }};

            for (auto& segment : tail())
                parts.push_back(std::move(segment));

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
                    Protecs::Components::BossPhase{ .phase = 1 },
                    ENRAGE,
                },
                .parts = std::move(parts),
            };
        }
    }

    using BossBlueprint = RType::Game::SpawnRequest (*)();

    constexpr std::array<std::pair<std::string_view, BossBlueprint>, 1> BOSSES = {{
        { "dobkeratops", &Dobkeratops::blueprint },
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
            wave.loot,
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
