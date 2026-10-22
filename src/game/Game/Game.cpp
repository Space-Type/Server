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
#include <variant>


namespace Packets = Protecs::Networking::Packets;


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

bool RType::Game::Game::pushEvent(const Protecs::Networking::Address& sender, Packets::PacketData&& data)
{
    if (std::holds_alternative<Packets::PlayerJoin>(data)) {
        auto& request = std::get<Packets::PlayerJoin>(data);
        if (!registerPlayer(sender, request.str()))
            return false;
    }

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

/**
 * Registers a new player in
 * the game. Returns true if
 * successful, and false if not.
 */
bool RType::Game::Game::registerPlayer(const Protecs::Networking::Address& address, std::string_view name)
{
    auto playerIt = m_Players.find(address);
    if (playerIt != m_Players.end()) {
        logger.warning() << "rejected request to join from " << address << " (already a player)" << std::endl;
        return false;
    }

    m_Players[address] = Player(address, name);
    logger.info() << "player \"" << name << "\" (" << address << ") joined the game" << std::endl;
    return true;
}

/**
 * Disconnects a player from
 * the current game (if possible).
 */
void RType::Game::Game::disconnectPlayer(const Protecs::Networking::Address& address)
{
    auto playerIt = m_Players.find(address);
    if (playerIt == m_Players.end()) {
        logger.warning() << "attempt to disconnect unknown player of address " << address << std::endl;
        return;
    }

    logger.info() << "player \"" << playerIt->second.name() << "\" (" << playerIt->second.address() << ") disconnected" << std::endl;
    m_Players.erase(playerIt);
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


/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//                                    Event Handler                                    //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////

RType::Game::Game::EventHandler::EventHandler(Game& game, Player& player) noexcept
    : m_Game(game)
    , m_Player(player)
{
}

void RType::Game::Game::EventHandler::operator()(const Packets::EntityAddComponent& data)
{
    logger.debug() << "received EntityAddComponent from " << m_Player.name() << std::endl;
}

void RType::Game::Game::EventHandler::operator()(const Packets::EntityComponentData& data)
{
    logger.debug() << "received EntityComponentData from " << m_Player.name() << std::endl;
}

void RType::Game::Game::EventHandler::operator()(const Packets::EntityRemoveComponent& data)
{
    logger.debug() << "received EntityRemoveComponent from " << m_Player.name() << std::endl;
}

void RType::Game::Game::EventHandler::operator()(const Packets::EntitySyncComponents& data)
{
    logger.debug() << "received EntitySyncComponents from " << m_Player.name() << std::endl;
}

void RType::Game::Game::EventHandler::operator()(const Packets::Ping& data)
{
    logger.debug() << "received Ping from " << m_Player.name() << std::endl;
}

void RType::Game::Game::EventHandler::operator()(const Packets::PlayerInput& data)
{
    logger.debug() << "received PlayerInput from " << m_Player.name() << std::endl;
}

void RType::Game::Game::EventHandler::operator()(const Packets::PlayerJoin& data)
{
    logger.debug() << "received PlayerJoin from " << m_Player.name() << std::endl;
    m_Game.m_OutgoingEvents.emplace_back();
}

void RType::Game::Game::EventHandler::operator()(const Packets::PlayerLeave& data)
{
    logger.debug() << "received PlayerLeave from " << m_Player.name() << std::endl;
}
