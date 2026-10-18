/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** ComponentManager type
*/

#pragma once

#include <rtype/components/types.hpp>
#include <wrecks/ecs.hpp>


namespace RType::Components {
    using Manager = Wrecks::ECS::ComponentManager<Transform>;
}
