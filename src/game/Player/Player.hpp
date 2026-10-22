/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Player class
*/

#pragma once

#include <protecs/networking.hpp>
#include <string>


namespace RType::Game {
    class Player {
        Protecs::Networking::Address m_Address;
        std::string m_Name;

        public:
            Player(const Protecs::Networking::Address& addr, std::string_view name) noexcept;
            Player() noexcept;

            inline const Protecs::Networking::Address& address() const noexcept { return m_Address; }
            inline std::string_view name() const noexcept { return m_Name; }
    };
}
