/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Game class
*/

#include "Game.hpp"
#include <rtype/game.hpp>
#include <rtype/utils.hpp>


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
    m_Players[sender] = Player(sender, "john");  // FIXME: remove
    // TODO: handle PlayerJoin event here, add player to map if valid

    auto playerIt = m_Players.find(sender);
    if (playerIt == m_Players.end()) {
        logger.warning() << "rejected request from " << sender << " (not a player)" << std::endl;
        return false;
    }

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
        auto& [player, data] = m_IncomingEvents.front();
        if (!player)
            continue;

        std::visit(EventHandler(*this, *player), data);

        m_IncomingEvents.pop_front();
    }
}
