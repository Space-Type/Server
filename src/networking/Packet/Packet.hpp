/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Packet struct
*/

#pragma once

#include "../packets/PacketData.hpp"
#include <cstdint>
#include <array>


namespace RType::Networking {
    struct Packet {
        std::array<std::uint8_t, sizeof(Packets::PacketData)> raw;

        static Packet serialize(const Packets::PacketData& data);
        Packets::PacketData deserialize() const;
    };
}
