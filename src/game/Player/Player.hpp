/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Player class
*/

#pragma once

#include <protecs/networking.hpp>


namespace RType::Game {
    class Player {
        Protecs::Networking::Address m_Address;

        public:
            Player(const Protecs::Networking::Address& addr) noexcept;
            Player() noexcept;

            const Protecs::Networking::Address& address() const noexcept { return m_Address; }
    };
}
