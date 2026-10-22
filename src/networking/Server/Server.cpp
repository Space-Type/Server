/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Server class
*/

#include "Server.hpp"
#include <protecs/errors.hpp>
#include <iostream>
#include <thread>



RType::Networking::Server::Server(std::string_view host, std::uint16_t port)
    : m_Socket()
    , m_Game()
    , m_PendingRequests()
{
    logger.debug() << "starting server" << std::endl;
    m_Socket.open(host, port);
    logger.info() << "started server on " << m_Socket.address() << std::endl;
}

RType::Networking::Server::Server(Server&& other)
    : m_Socket()
    , m_Game()
    , m_PendingRequests()
{
    swap(other);
}

RType::Networking::Server::~Server()
{
    logger.info() << "shut down" << std::endl;
}

void RType::Networking::Server::run()
{
    Utils::InterruptHandler::initialize();

    std::jthread pollThread([&](){
        while (m_Socket.isOpen()) {
            receiveRequests();
        }
    });

    while (m_Socket.isOpen()) {
        try {
            update();
        } catch (const Protecs::Error& err) {
            logger.error() << err.what() << std::endl;
        }

        m_Game.update();

        if (Utils::InterruptHandler::isActive()) {
            logger.fatal() << "CTRL + C received, shutting down..." << std::endl;
            m_Socket.close();
            Utils::InterruptHandler::reset();
        }
    }
}

void RType::Networking::Server::update()
{
    // Send all outgoing events
    Game::Game::Event evt;

    while (m_Game.popEvent(evt)) {
        auto& [player, data] = evt;

        auto packet = Protecs::Networking::Packet::serialize(data);
        m_Socket.send(packet, player->address());
    }

    // Handle all incoming events
    auto request = m_PendingRequests.pop();

    while (request) {
        auto& [sender, packet] = *request;
        m_Game.pushEvent(sender, std::move(packet));

        request = m_PendingRequests.pop();
    }

}

void RType::Networking::Server::swap(Server& other)
{
    std::swap(m_Socket, other.m_Socket);
}

void RType::Networking::Server::receiveRequests()
{
    if (!m_Socket.pollReceive(POLL_TIMEOUT))
        return;

    Protecs::Networking::Address senderAddr;
    Protecs::Networking::Packet packet = {};

    m_Socket.receive(packet, &senderAddr);
    logger.debug() << "received packet from " << senderAddr << " (" << packet.filled << "B)" << std::endl;
    m_PendingRequests.push({ senderAddr, packet.deserialize() });
}

/////////////////////////////////////////////////////////
//                                                     //
//                       Builder                       //
//                                                     //
/////////////////////////////////////////////////////////

RType::Networking::Server::Builder::Builder() noexcept
    : m_Host("0.0.0.0")
    , m_Port(8080)
{
}

auto RType::Networking::Server::Builder::setHost(std::string_view host) && -> Builder&&
{
    m_Host = host == "localhost" ? "127.0.0.1" : host;
    return std::move(*this);
}

auto RType::Networking::Server::Builder::setPort(std::uint16_t port) && -> Builder&&
{
    m_Port = port;
    return std::move(*this);
}

auto RType::Networking::Server::Builder::setLogStream(std::ostream& output) && -> Builder&&
{
    logger.setOutput(output);
    return std::move(*this);
}

auto RType::Networking::Server::Builder::setLogLevel(Utils::Logger::Level level) && -> Builder&&
{
    logger.setLevel(level);
    return std::move(*this);
}

auto RType::Networking::Server::Builder::build() && -> Server
{
    return Server(m_Host, m_Port);
}
