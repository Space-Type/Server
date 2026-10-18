/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Server class
*/

#pragma once

#include <wrecks/networking.hpp>
#include <string_view>
#include <cstdint>
#include <ostream>
#include <iomanip>
#include <ctime>


namespace RType::Networking {
    class Server {
        Wrecks::Networking::UDPSocket m_Socket;
        std::ostream* m_Output;

        private:
            Server(std::string_view host, std::uint16_t port, std::ostream* logStream = nullptr);

        public:
            Server(const Server& cpy) = delete;
            Server(Server&& other);
            ~Server();

            void run();
            void update();

            inline void setLogStream(std::ostream& output) noexcept { m_Output = &output; }

            void swap(Server& other);
            inline void operator=(Server&& other) { swap(other); }

        private:
            std::ostream& log(std::string_view tag = "LOG");

        public:
            class Builder {
                std::string_view m_Host;
                std::uint16_t m_Port;
                std::ostream* m_OutputStream;

                public:
                    Builder() noexcept;

                    Builder&& setHost(std::string_view host) &&;
                    Builder&& setPort(std::uint16_t port) &&;
                    Builder&& setLogStream(std::ostream& output) &&;
                    Builder&& setLogStream() &&;

                    Server build() &&;
            };
    };
}
