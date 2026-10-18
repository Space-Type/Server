/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Packet struct
*/

#include "Packet.hpp"
#include <wrecks.hpp>


auto RType::Networking::Packet::serialize(const Packets::PacketData& data) -> Packet
{
    Packet packet = {};

    auto dataStart = packet.raw.begin();
    *dataStart = static_cast<std::uint8_t>(data.index());
    ++dataStart;

    std::visit([&](const auto& data){
        data.serialize(std::span(dataStart, packet.raw.end()));
    }, data);

    return packet;
}

auto RType::Networking::Packet::deserialize() const -> Packets::PacketData
{
    using Wrecks::Utils::element_index_v;
    Packets::PacketData result;

    auto dataStart = raw.cbegin();
    std::uint8_t type = *dataStart;
    ++dataStart;

    switch (type) {
        case element_index_v<Packets::EntityPosition, Packets::PacketData>:
            result = Packets::EntityPosition::deserialize(std::span(dataStart, raw.cend()));
            break;

        case element_index_v<Packets::PlayerInput, Packets::PacketData>:
            result = Packets::PlayerInput::deserialize(std::span(dataStart, raw.cend()));
            break;

        default: throw std::runtime_error("unknown packet data type " + std::to_string(type));
    }

    return result;
}
