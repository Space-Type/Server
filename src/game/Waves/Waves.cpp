/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** stage waves loaded from data files
*/

#include "Waves.hpp"
#include <rtype/utils.hpp>
#include <protecs/common.hpp>
#include <algorithm>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <optional>
#include <sstream>


namespace {
    using EntityCreate = Protecs::Networking::Packets::EntityCreate;

    std::optional<EntityCreate::EntityType> parseType(std::string_view name) noexcept
    {
        if (name == "pata-pata")
            return EntityCreate::ENEMY_PATA_PATA;
        return std::nullopt;
    }

    std::optional<RType::Components::Pattern> parsePattern(std::string_view name) noexcept
    {
        if (name == "straight")
            return RType::Components::Pattern::Straight;
        if (name == "sine")
            return RType::Components::Pattern::Sine;
        return std::nullopt;
    }

    template <typename T>
    std::optional<T> parseNumber(const std::string& text) noexcept
    {
        T value{};
        auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);

        if (error != std::errc() || end != text.data() + text.size())
            return std::nullopt;
        return value;
    }
}


auto RType::Game::parseWaves(std::string_view text, std::vector<std::string>& errors) -> std::vector<Wave>
{
    std::vector<Wave> waves;
    std::istringstream lines{ std::string(text) };
    std::string line;

    for (std::size_t number = 1; std::getline(lines, line); ++number) {
        std::istringstream words(line);
        std::string progression, type, y, pattern, extra;

        if (!(words >> progression) || progression.starts_with('#'))
            continue;

        auto value = parseNumber<std::uint32_t>(progression);
        auto entity = (words >> type) ? parseType(type) : std::nullopt;
        auto height = (words >> y) ? parseNumber<float>(y) : std::nullopt;
        auto movement = (words >> pattern) ? parsePattern(pattern) : std::nullopt;

        if (!value || !entity || !height || !movement || (words >> extra && !extra.starts_with('#'))
            || *height < 0.0F || *height > Protecs::MAP_MAX_Y) {
            errors.push_back("line " + std::to_string(number) + ": expected \"progression type y movement\", got \"" + line + "\"");
            continue;
        }
        waves.push_back({ .progression = *value, .type = *entity, .y = *height, .pattern = *movement });
    }
    std::ranges::stable_sort(waves, {}, &Wave::progression);
    return waves;
}

auto RType::Game::loadWaves(const std::string& name) noexcept -> std::vector<Wave>
{
    try {
        std::error_code error;
        std::filesystem::path directory = std::filesystem::current_path(error);

        for (; !directory.empty(); directory = directory.parent_path()) {
            std::filesystem::path path = directory / "assets" / "stages" / name;

            if (!std::filesystem::is_regular_file(path, error)) {
                if (directory == directory.parent_path())
                    break;
                continue;
            }
            if (std::filesystem::file_size(path, error) > WAVES_FILE_MAX) {
                logger.error() << "stage file " << path << " is too big, no waves loaded" << std::endl;
                return {};
            }

            std::ifstream file(path, std::ios::binary);
            std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            std::vector<std::string> errors;
            std::vector<Wave> waves = parseWaves(text, errors);

            for (const auto& message : errors)
                logger.warning() << path.filename().string() << ": " << message << std::endl;
            logger.info() << "loaded " << waves.size() << " waves from " << path << std::endl;
            return waves;
        }
        logger.error() << "stage file assets/stages/" << name << " not found, no waves loaded" << std::endl;
    } catch (const std::exception& err) {
        logger.error() << "cannot load stage file " << name << ": " << err.what() << std::endl;
    }
    return {};
}
