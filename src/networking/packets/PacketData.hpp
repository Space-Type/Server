/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** PacketData variant
*/

#pragma once

#include <rtype/networking/packets.hpp>
#include <variant>


namespace RType::Networking::Packets {
    using PacketData = std::variant<EntityPosition>;
}
