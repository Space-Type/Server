/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** NetworkID component
*/

#include "NetworkID.hpp"
#include <protecs/networking.hpp>


auto RType::Components::NetworkID::fromEntity(Wrecks::ECS::Entity entity) noexcept -> NetworkID
{
    return { Protecs::Networking::generateNetworkEntity(entity) };
}
