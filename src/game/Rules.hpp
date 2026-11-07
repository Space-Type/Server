/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** gameplay rules shared by the systems
*/

#pragma once

#include <protecs/common.hpp>
#include <protecs/networking.hpp>
#include <array>
#include <cstdint>


namespace RType::Rules {
    using EntityType = Protecs::Networking::Packets::EntityCreate::EntityType;
    using Vec2D = Protecs::Utils::Vec2D<float>;
    using Protecs::Networking::Packets::EntityCreate;

    // Sizes match the client sprites (world units, drawn 1:1)
    inline constexpr Vec2D PLAYER_SIZE = { Protecs::PLAYER_WIDTH, Protecs::PLAYER_HEIGHT };
    inline constexpr Vec2D ENEMY_SIZE = { 33.0F, 36.0F };
    inline constexpr Vec2D PLAYER_MISSILE_SIZE = { 33.0F, 10.0F };
    inline constexpr Vec2D ENEMY_MISSILE_SIZE = { 9.0F, 8.0F };

    inline constexpr float PLAYER_SHOT_COOLDOWN = 0.25F;  // seconds
    inline constexpr float PLAYER_MISSILE_SPEED = 300.0F;  // world units per second
    inline constexpr float ENEMY_SPEED = 60.0F;
    inline constexpr float ENEMY_MISSILE_SPEED = 150.0F;
    inline constexpr float ENEMY_SHOT_MIN = 1.5F;          // seconds between two shots of an enemy
    inline constexpr float ENEMY_SHOT_MAX = 3.5F;

    // Charged shot (R-Type's Wave Cannon): hold the trigger, release to fire
    inline constexpr float CHARGE_LEVEL_TIME = 0.4F;      // seconds per charge level
    inline constexpr std::uint8_t CHARGE_LEVELS = 5;
    inline constexpr float BEAM_SPEED = 400.0F;
    inline constexpr std::array<Vec2D, CHARGE_LEVELS> BEAM_SIZES = {{  // client beam sprites (r-typesheet1)
        { 17.0F, 12.0F }, { 33.0F, 12.0F }, { 49.0F, 14.0F }, { 65.0F, 14.0F }, { 81.0F, 16.0F },
    }};

    inline constexpr std::uint8_t MISSILE_DAMAGE = 1;
    inline constexpr std::uint16_t PATA_PATA_HEALTH = 1;

    inline constexpr std::uint8_t PLAYER_LIVES = 3;
    inline constexpr std::uint32_t ENEMY_SCORE = 100;
    inline constexpr float RESPAWN_INVULNERABILITY = 2.0F;  // the client blinks the ship as long (Game-Client: BLINK_DURATION)
    inline constexpr float OFFSCREEN_MARGIN = 64.0F;        // beyond it, an enemy or a missile is gone

    // Levels: the progression is in pixels of the stage background (Game-Client: assets/stages, 240 px high)
    inline constexpr float SCROLL_SPEED = 30.0F;           // background pixels per second
    inline constexpr float VISIBLE_BACKGROUND = 240.0F * Protecs::MAP_MAX_X / Protecs::MAP_MAX_Y;  // ~427 px on screen
    // The stage ends when the end of stage-1.png (4224 px wide) reaches the right edge of the screen
    inline constexpr std::uint32_t STAGE1_LENGTH = 4224 - static_cast<std::uint32_t>(VISIBLE_BACKGROUND);

    // Pattern::Sine
    inline constexpr float SINE_AMPLITUDE = 40.0F;         // world units above and below the starting height
    inline constexpr float SINE_PERIOD = 2.0F;             // seconds

    // Stage 1 boss (Dobkeratops, without its tail). Positions in pixels of its sprite (r-typesheet30, 155 x 204)
    inline constexpr float BOSS_SCALE = 1.6F;                        // ~70 % of the screen height
    inline constexpr Vec2D BOSS_SPRITE = { 155.0F, 204.0F };
    inline constexpr Vec2D BOSS_BODY_OFFSET = { 95.0F, 40.0F };      // solid part: hurts on contact, absorbs shots
    inline constexpr Vec2D BOSS_BODY_SIZE = { 60.0F, 160.0F };
    inline constexpr Vec2D BOSS_CORE_OFFSET = { 72.0F, 95.0F };      // the creature in its chest: the weak point
    inline constexpr Vec2D BOSS_CORE_SIZE = { 28.0F, 30.0F };
    inline constexpr Vec2D BOSS_MOUTH = { 5.0F, 55.0F };             // where its shots leave
    inline constexpr float BOSS_SPEED = 40.0F;                       // entering from the right
    inline constexpr float BOSS_MARGIN = 10.0F;                      // from the right edge once in place
    inline constexpr std::uint16_t BOSS_HEALTH = 60;
    inline constexpr std::uint32_t BOSS_SCORE = 5000;
    inline constexpr std::uint8_t BOSS_BURST = 3;                    // aimed shots per burst
    inline constexpr float BOSS_SHOT_INTERVAL = 0.25F;               // seconds between the shots of a burst
    inline constexpr float BOSS_BURST_INTERVAL = 2.0F;               // seconds between two bursts
    inline constexpr float BOSS_SHOT_SPEED = 120.0F;

    // Every enemy part: they hurt players on contact
    constexpr bool isEnemy(EntityType type) noexcept
    {
        return type == EntityCreate::ENEMY_PATA_PATA || type == EntityCreate::ENEMY_SCANT
            || type == EntityCreate::ENEMY_DOBKERATOPS || type == EntityCreate::ENEMY_DOBKERATOPS_CORE;
    }

    // Small enemies, which shoot at random intervals (the boss has its own system)
    constexpr bool shootsRandomly(EntityType type) noexcept
    {
        return type == EntityCreate::ENEMY_PATA_PATA || type == EntityCreate::ENEMY_SCANT;
    }

    // Charge level (0 to CHARGE_LEVELS) after holding the trigger for `seconds`
    constexpr std::uint8_t chargeLevel(float seconds) noexcept
    {
        auto level = static_cast<int>(seconds / CHARGE_LEVEL_TIME);

        return static_cast<std::uint8_t>(level < 0 ? 0 : (level > CHARGE_LEVELS ? CHARGE_LEVELS : level));
    }

    constexpr bool isBeam(EntityType type) noexcept
    {
        return type >= EntityCreate::PLAYER_BEAM_1 && type <= EntityCreate::PLAYER_BEAM_5;
    }

    // Level 1 to 5: a beam deals one more damage than its level
    constexpr EntityType beamType(std::uint8_t level) noexcept
    {
        return static_cast<EntityType>(EntityCreate::PLAYER_BEAM_1 + level - 1);
    }

    constexpr std::uint8_t beamDamage(std::uint8_t level) noexcept
    {
        return static_cast<std::uint8_t>(level + 1);
    }

    constexpr bool overlap(Vec2D a, Vec2D aSize, Vec2D b, Vec2D bSize) noexcept
    {
        return a.x < b.x + bSize.x && b.x < a.x + aSize.x && a.y < b.y + bSize.y && b.y < a.y + aSize.y;
    }

    constexpr Vec2D respawnPosition(std::uint8_t slot) noexcept
    {
        return { 40.0F, 60.0F + 90.0F * static_cast<float>(slot) };
    }

    /**
     * Small random generator (xorshift32) for the systems,
     * which must be nothrow default constructible.
     */
    class Random {
        std::uint32_t m_State = 2463534242U;

        public:
            inline void seed(std::uint32_t seed) noexcept { m_State = seed != 0 ? seed : 2463534242U; }

            // Uniform in [min, max)
            inline float between(float min, float max) noexcept
            {
                m_State ^= m_State << 13;
                m_State ^= m_State >> 17;
                m_State ^= m_State << 5;
                return min + (max - min) * static_cast<float>(m_State >> 8) / static_cast<float>(1U << 24);
            }
    };
}
