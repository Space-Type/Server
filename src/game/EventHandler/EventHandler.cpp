/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** EventHandler class
*/

#include "EventHandler.hpp"
#include "../Game/Game.hpp"
#include <rtype/utils.hpp>


namespace Packets = Protecs::Networking::Packets;


RType::Game::EventHandler::EventHandler(Game& game, Player& player) noexcept
    : m_Game(game)
    , m_Player(player)
{
}

void RType::Game::EventHandler::operator()(const Packets::EntityAddComponent& data)
{
    logger.debug() << "received EntityAddComponent from " << m_Player.name() << std::endl;
}

void RType::Game::EventHandler::operator()(const Packets::EntityComponentData& data)
{
    logger.debug() << "received EntityComponentData from " << m_Player.name() << std::endl;
}

void RType::Game::EventHandler::operator()(const Packets::EntityRemoveComponent& data)
{
    logger.debug() << "received EntityRemoveComponent from " << m_Player.name() << std::endl;
}

void RType::Game::EventHandler::operator()(const Packets::EntitySyncComponents& data)
{
    logger.debug() << "received EntitySyncComponents from " << m_Player.name() << std::endl;
}

void RType::Game::EventHandler::operator()(const Packets::PlayerInput& data)
{
    logger.debug() << "received PlayerInput from " << m_Player.name() << std::endl;
}
