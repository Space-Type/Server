/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Server class
*/

#include "Server.hpp"
#include "../Packet/Packet.hpp"
#include <rtype/utils.hpp>
#include <iostream>


static class : public std::ostream {} VOID_STREAM;

RType::Networking::Server::Server(std::string_view host, std::uint16_t port, std::ostream* output)
    : m_Socket()
    , m_Output(output)
{
    log() << "starting server" << std::endl;
    m_Socket.open(host, port);
    log() << "started server on " << m_Socket.address() << std::endl;
}

RType::Networking::Server::Server(Server&& other)
    : m_Socket()
    , m_Output(nullptr)
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

    while (m_Socket.isOpen()) {
        update();

        if (Utils::InterruptHandler::isActive()) {
            log() << "CTRL + C received, shutting down..." << std::endl;
            m_Socket.close();
            Utils::InterruptHandler::reset();
        }
    }
}

void RType::Networking::Server::update()
{
    Wrecks::Networking::Address senderAddr;
    Packet packet = {};

    auto bytes = m_Socket.receive(packet, &senderAddr);
    log() << "received packet from " << senderAddr << " (" << bytes << "B)" << std::endl;

    auto data = packet.deserialize();
    auto& pos = std::get<Packets::EntityPosition>(data);

    std::cout << "{ "
              << ".entity = " << pos.entity << ", "
              << ".x = " << pos.x << ", "
              << ".y = " << pos.y << " "
              << "}" << std::endl;
}

void RType::Networking::Server::swap(Server& other)
{
    std::swap(m_Socket, other.m_Socket);
}

auto RType::Networking::Server::log(std::string_view tag) -> std::ostream&
{
    if (!m_Output)
        return VOID_STREAM;

    auto time = std::time(nullptr);
    auto localtime = *std::localtime(&time);

    return *m_Output << std::put_time(&localtime, "%d-%m-%Y %H:%M:%S") << " "
                     << "[\033[34;1m" << tag << "\033[0m] "
                     << "- server: ";
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
