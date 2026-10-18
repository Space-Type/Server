/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Program Entry
*/

#include "networking/Server/Server.hpp"
#include <rtype.hpp>
#include <wrecks.hpp>


int main(int argc, char *argv[])
{
    auto server = RType::Networking::Server::Builder()
        .setHost("0.0.0.0")
        .setPort(5432)
        .build();

    server.run();
}
