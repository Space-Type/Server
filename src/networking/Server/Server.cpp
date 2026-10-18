/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Server class
*/

#include "Server.hpp"
#include <protecs/errors.hpp>
#include <rtype/utils.hpp>
#include <iostream>
#include <iomanip>
#include <thread>
#include <ctime>


// Discards everything written to it (no stream buffer)
static std::ostream VOID_STREAM(nullptr);


RType::Networking::Server::Server(std::string_view host, std::uint16_t port, std::ostream* output)
    : m_Socket()
    , m_Output(output)
    , m_PendingRequests()
{
    log() << "starting server" << std::endl;
    m_Socket.open(host, port);
    log() << "started server on " << m_Socket.address() << std::endl;
}

RType::Networking::Server::Server(Server&& other)
    : m_Socket()
    , m_Output(nullptr)
    , m_PendingRequests()
{
    swap(other);
}

RType::Networking::Server::~Server()
{
    log() << "shut down" << std::endl;
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
            log("ERROR") << err.what() << std::endl;
        }

        if (Utils::InterruptHandler::isActive()) {
            log("WARNING") << "CTRL + C received, shutting down..." << std::endl;
            m_Socket.close();
            Utils::InterruptHandler::reset();
        }
    }
}

void RType::Networking::Server::update()
{
    auto request = m_PendingRequests.pop();
    if (!request)
        return;

    auto& [sender, packet] = *request;

    auto data = packet.deserialize();
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

    auto bytes = m_Socket.receive(packet, &senderAddr);
    log() << "received packet from " << senderAddr << " (" << bytes << "B)" << std::endl;
    m_PendingRequests.push({ senderAddr, std::move(packet) });
}

auto RType::Networking::Server::log(std::string_view tag) -> std::ostream&
{
    if (!m_Output)
        return VOID_STREAM;

    auto time = std::time(nullptr);
    auto localtime = *std::localtime(&time);

    return *m_Output << std::put_time(&localtime, "%d-%m-%Y %H:%M:%S") << " "
                     << "[\033[" << tagColor(tag) << ";1m" << tag << "\033[0m] "
                     << "- server: ";
}

auto RType::Networking::Server::tagColor(std::string_view tag) -> unsigned int
{
    if (tag == "WARNING") return 33;  // yellow-ish orange
    if (tag == "ERROR")   return 31;    // red
    if (tag == "FATAL")   return 35;    // magenta

    return 34;  // blue
}

/////////////////////////////////////////////////////////
//                                                     //
//                       Builder                       //
//                                                     //
/////////////////////////////////////////////////////////

RType::Networking::Server::Builder::Builder() noexcept
    : m_Host("0.0.0.0")
    , m_Port(8080)
    , m_OutputStream(&std::cout)
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
    m_OutputStream = &output;
    return std::move(*this);
}

auto RType::Networking::Server::Builder::setLogStream() && -> Builder&&
{
    m_OutputStream = nullptr;
    return std::move(*this);
}

auto RType::Networking::Server::Builder::build() && -> Server
{
    return Server(m_Host, m_Port, m_OutputStream);
}
