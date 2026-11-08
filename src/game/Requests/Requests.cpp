/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** entity requests made by the systems
*/

#include "Requests.hpp"
#include <algorithm>


void RType::Game::Requests::destroy(Wrecks::ECS::Entity entity)
{
    if (!destroying(entity))
        destroys.push_back(entity);
}

void RType::Game::Requests::kill(Wrecks::ECS::Entity entity)
{
    destroy(entity);
    if (!killed(entity))
        kills.push_back(entity);
}

bool RType::Game::Requests::killed(Wrecks::ECS::Entity entity) const noexcept
{
    return std::ranges::find(kills, entity) != kills.end();
}

bool RType::Game::Requests::destroying(Wrecks::ECS::Entity entity) const noexcept
{
    return std::ranges::find(destroys, entity) != destroys.end();
}
