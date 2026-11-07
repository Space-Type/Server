/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** LevelState component
*/

#pragma once


namespace RType::Components {
    // Server-only state of the level entity, next to its shared Protecs::Components::Level
    struct LevelState {
        float distance = 0.0F;     // background pixels scrolled in the current stage (Level::progression, as a float)
        float spawnTimer = 1.0F;   // seconds before the next enemy
    };
}
