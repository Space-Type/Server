/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** StopAt component
*/

#pragma once

namespace RType::Components {
    // Flies left until its position reaches `x`, then stays there (a boss entering from the right)
    struct StopAt {
        float x = 0.0F;
    };
}
