/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Player class
*/

#include "Player.hpp"


RType::Game::Player::Player(const Protecs::Networking::Address& addr, std::string_view name) noexcept
    : m_Address(addr)
    , m_Name(name)
{

}

RType::Game::Player::Player() noexcept
    : m_Address()
    , m_Name()
{

}
