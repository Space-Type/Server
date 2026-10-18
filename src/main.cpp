/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Program Entry
*/

#include <rtype.hpp>
#include <wrecks.hpp>
#include <iostream>


static void run(int argc, char *argv[])
{
    auto server = RType::Networking::Server::Builder()
        .setHost("0.0.0.0")
        .setPort(5432)
        .build();

    server.run();
}

int main(int argc, char *argv[])
{
    try {
        run(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
    }
}
