/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Program Entry
*/

#include <rtype.hpp>
#include <iostream>
#include <stdexcept>


static void run(const RType::Game::DevOptions& dev)
{
    auto server = RType::Game::Server::Builder()
        .setHost("0.0.0.0")
        .setPort(5432)
        .setMaxPlayers(4)
        .setDevOptions(dev)
        .build();

    server.run();
}

int main(int argc, char *argv[])
{
    RType::Game::CommandLine line;

    try {
        line = RType::Game::parseCommandLine(argc, argv);
    } catch (const std::invalid_argument& err) {
        std::cerr << err.what() << "\n\n" << RType::Game::USAGE;
        return 84;
    }
    if (line.help) {
        std::cout << RType::Game::USAGE;
        return 0;
    }

    try {
        run(line.dev);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 84;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 84;
    }
}
