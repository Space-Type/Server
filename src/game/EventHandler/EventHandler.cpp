/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** EventHandler class
*/

#include "EventHandler.hpp"
#include "../Game/Game.hpp"


namespace Packets = Protecs::Networking::Packets;


RType::Game::EventHandler::EventHandler(Game& game, const Protecs::Networking::Address& sender) noexcept
    : m_Game(game)
    , m_Sender(sender)
{
}

void RType::Game::EventHandler::operator()(const Packets::EntityAddComponent& data)
{

}

void RType::Game::EventHandler::operator()(const Packets::EntityComponentData& data)
{

}

void RType::Game::EventHandler::operator()(const Packets::EntityRemoveComponent& data)
{

}

void RType::Game::EventHandler::operator()(const Packets::EntitySyncComponents& data)
{

}

void RType::Game::EventHandler::operator()(const Packets::PlayerInput& data)
{

}
