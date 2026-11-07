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
