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
    inline constexpr float ENEMY_SPAWN_MIN = 1.0F;         // seconds between two enemies
    inline constexpr float ENEMY_SPAWN_MAX = 2.0F;
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

    // Levels: the progression is in pixels of the stage background (Game-Client: assets/stages)
    inline constexpr float SCROLL_SPEED = 30.0F;           // background pixels per second
    inline constexpr std::uint32_t STAGE1_LENGTH = 4224;   // width of stage-1.png

    constexpr bool isEnemy(EntityType type) noexcept
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
