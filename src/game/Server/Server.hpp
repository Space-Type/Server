/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Server class
*/

#pragma once

#include "../Game/Game.hpp"
#include "../DevOptions/DevOptions.hpp"
#include <rtype/utils.hpp>
#include <protecs/networking.hpp>
#include <string_view>
#include <cstdint>


namespace RType::Game {
    class Server {
        static constexpr int POLL_TIMEOUT = 1000;

        Protecs::Networking::UDPSocket m_Socket;
        Game m_Game;
        Protecs::Networking::PacketQueue m_PendingRequests;

        private:
            Server(std::string_view host, std::uint16_t port, const DevOptions& dev);

        public:
            Server(const Server& cpy) = delete;
            Server(Server&& other);
            ~Server();

            void run();
            void update(Wrecks::DeltaTime dt);

            void swap(Server& other);
            inline void operator=(Server&& other) { swap(other); }

        private:
            void receiveRequests();
            void sendOutgoingEvents();
            void handleIncomingEvents(Protecs::Tick simulationTick);

        public:
            class Builder {
                std::string_view m_Host;
                std::uint16_t m_Port;
                DevOptions m_Dev;
                unsigned int m_MaxPlayers;

                public:
                    Builder() noexcept;

                    Builder&& setHost(std::string_view host) &&;
                    Builder&& setPort(std::uint16_t port) &&;
                    Builder&& setMaxPlayers(unsigned int count) &&;
                    Builder&& setLogStream(std::ostream& output) &&;
                    Builder&& setLogLevel(Utils::Logger::Level level) &&;
                    Builder&& setDevOptions(const DevOptions& dev) &&;

                    Server build() &&;
            };
    };
}
