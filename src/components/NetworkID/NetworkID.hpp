/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** NetworkID component
*/

#pragma once

#include <protecs/common.hpp>


namespace RType::Components {
    struct NetworkID {
        Protecs::NetworkEntity id;

        static NetworkID fromEntity(Wrecks::ECS::Entity entity) noexcept;
    };
}
