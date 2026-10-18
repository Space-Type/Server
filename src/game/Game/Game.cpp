/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Game class
*/

#include "Game.hpp"


RType::Game::Game::Game() noexcept
    : m_Engine()
    , m_Players()
    , m_IncomingEvents()
    , m_OutgoingEvents()
{

}

void RType::Game::Game::update()
{
    handleEvents();
    m_Engine.update();
}

bool RType::Game::Game::pushEvent(const Protecs::Networking::Address& sender, Protecs::Networking::Packets::PacketData&& data)
{
    auto playerIt = m_Players.find(sender);
    if (playerIt == m_Players.end())
        return false;

    m_IncomingEvents.emplace_back(&playerIt->second, std::move(data));
    return true;
}

bool RType::Game::Game::popEvent(Event& data)
{
    if (m_OutgoingEvents.empty())
        return false;

    data = std::move(m_OutgoingEvents.front());
    m_OutgoingEvents.pop_front();

    return true;
}

void RType::Game::Game::handleEvents()
{
    while (!m_IncomingEvents.empty()) {
        const auto& [player, data] = m_IncomingEvents.front();

        // TODO: handle event

        m_IncomingEvents.pop_front();
    }
}
