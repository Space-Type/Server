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



RType::Game::Server::Server(std::string_view host, std::uint16_t port, const DevOptions& dev)
    : m_Socket()
    , m_Game()
    , m_PendingRequests()
{
    logger.debug() << "starting server" << std::endl;
    m_Socket.open(host, port);
    logger.info() << "started server on " << m_Socket.address() << std::endl;

    m_Game.setEventRetriever([&](Protecs::Tick simulationTick){ handleIncomingEvents(simulationTick); });
    m_Game.setDevOptions(dev);
}

RType::Game::Server::Server(Server&& other)
    : m_Socket()
    , m_Game()
    , m_PendingRequests()
{
    swap(other);
}

RType::Game::Server::~Server()
{
    logger.info() << "shut down" << std::endl;
}

void RType::Game::Server::run()
{
    Utils::InterruptHandler::initialize();

    std::jthread pollThread([&](){
        while (m_Socket.isOpen()) {
            receiveRequests();
        }
    });

    std::chrono::time_point frameStart = std::chrono::steady_clock::now();
    Wrecks::DeltaTime delta(0.001);

    while (m_Socket.isOpen()) {
        try {
            update(delta);
        } catch (const std::exception& err) {
            logger.error() << err.what() << std::endl;
        }

        std::chrono::time_point frameEnd = std::chrono::steady_clock::now();
        delta = frameEnd - frameStart;
        frameStart = frameEnd;

        if (Utils::InterruptHandler::isActive()) {
            logger.fatal() << "CTRL + C received, shutting down..." << std::endl;
            m_Socket.close();
            Utils::InterruptHandler::reset();
        }
    }
}

void RType::Game::Server::update(Wrecks::DeltaTime dt)
{
    sendOutgoingEvents();
    m_Game.update(dt);
}

void RType::Game::Server::swap(Server& other)
{
    std::swap(m_Socket, other.m_Socket);
    std::swap(m_Game, other.m_Game);
    std::swap(m_PendingRequests, other.m_PendingRequests);
}

/**
 * Receives packets from anyone and
 * stores them in the pending requests
 * buffer.
 *
 * Note:
 * This function should only be called
 * in a separate thread.
 */
void RType::Game::Server::receiveRequests()
{
    if (!m_Socket.pollReceive(POLL_TIMEOUT))
        return;

    Protecs::Networking::Address senderAddr;
    Protecs::Networking::Packet packet = {};

    m_Socket.receive(packet, &senderAddr);
    logger.trace() << "received " << packet << " from " << senderAddr << std::endl;
    m_PendingRequests.push(senderAddr, packet);
}

/**
 * Sends all queued outgoing
 * events to their recipients.
 */
void RType::Game::Server::sendOutgoingEvents()
{
    Game::Game::Event evt;

    while (m_Game.popEvent(evt)) {
        auto& [address, data] = evt;

        auto packet = Protecs::Networking::Packet::serialize(data);
        logger.trace() << "sending " << packet << " to " << address << std::endl;
        m_Socket.send(packet, address);
    }
}

/**
 * Handles all incoming events up to
 * the current simulation tick.
 */
void RType::Game::Server::handleIncomingEvents(Protecs::Tick simulationTick)
{
    auto request = m_PendingRequests.pop(simulationTick);
    while (request) {
        auto& [sender, packet] = *request;

        try {
            m_Game.pushEvent(sender, packet.deserialize());
        } catch (const Protecs::SerializationError& err) {
            logger.warning() << "failed to deserialize packet from " << sender << std::endl;
        }

        request = m_PendingRequests.pop(simulationTick);
    }
}


/////////////////////////////////////////////////////////
//                                                     //
//                       Builder                       //
//                                                     //
/////////////////////////////////////////////////////////

RType::Game::Server::Builder::Builder() noexcept
    : m_Host("0.0.0.0")
    , m_Port(8080)
    , m_Dev()
    , m_MaxPlayers(4)
{
}

auto RType::Game::Server::Builder::setHost(std::string_view host) && -> Builder&&
{
    m_Host = host == "localhost" ? "127.0.0.1" : host;
    return std::move(*this);
}

auto RType::Game::Server::Builder::setPort(std::uint16_t port) && -> Builder&&
{
    m_Port = port;
    return std::move(*this);
}

auto RType::Game::Server::Builder::setMaxPlayers(unsigned int count) && -> Builder&&
{
    m_MaxPlayers = count;
    return std::move(*this);
}

auto RType::Game::Server::Builder::setLogStream(std::ostream& output) && -> Builder&&
{
    logger.setOutput(output);
    return std::move(*this);
}

auto RType::Game::Server::Builder::setLogLevel(Utils::Logger::Level level) && -> Builder&&
{
    logger.setLevel(level);
    return std::move(*this);
}

auto RType::Game::Server::Builder::setDevOptions(const DevOptions& dev) && -> Builder&&
{
    m_Dev = dev;
    return std::move(*this);
}

auto RType::Game::Server::Builder::build() && -> Server
{
    Server server(m_Host, m_Port, m_Dev);

    server.m_Game.setMaxPlayers(m_MaxPlayers);

    return server;
}
