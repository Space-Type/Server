/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** stage waves loaded from data files
*/

#include "Waves.hpp"
#include "../Blueprints/Blueprints.hpp"
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


auto RType::Game::parseStage(std::string_view text, std::vector<std::string>& errors) -> Stage
{
    Stage stage;
    std::istringstream lines{ std::string(text) };
    std::string line;

    for (std::size_t number = 1; std::getline(lines, line); ++number) {
        std::istringstream words(line);
        std::string first, second, third, pattern, extra;

        if (!(words >> first) || first.starts_with('#'))
            continue;

        bool more = static_cast<bool>(words >> second);
        bool tooMany = static_cast<bool>(words >> third) && !third.starts_with('#');

        if (first == "length" || first == "boss") {
            auto length = first == "length" && more ? parseNumber<std::uint32_t>(second) : std::nullopt;

            if (!more || tooMany || (first == "length" && !length) || (first == "boss" && !Blueprints::isBoss(second))) {
                errors.push_back("line " + std::to_string(number) + ": expected \"length <n>\" or \"boss <name>\", got \"" + line + "\"");
                continue;
            }
            if (length)
                stage.length = *length;
            else
                stage.boss = second;
            continue;
        }

        auto value = parseNumber<std::uint32_t>(first);
        auto entity = more ? parseType(second) : std::nullopt;
        auto height = tooMany ? parseNumber<float>(third) : std::nullopt;
        auto movement = (words >> pattern) ? parsePattern(pattern) : std::nullopt;

        if (!value || !entity || !height || !movement || (words >> extra && !extra.starts_with('#'))
            || *height < 0.0F || *height > Protecs::MAP_MAX_Y) {
            errors.push_back("line " + std::to_string(number) + ": expected \"progression type y movement\", got \"" + line + "\"");
            continue;
        }
        stage.waves.push_back({ .progression = *value, .type = *entity, .y = *height, .pattern = *movement });
    }
    std::ranges::stable_sort(stage.waves, {}, &Wave::progression);
    return stage;
}

auto RType::Game::loadStage(const std::string& name) noexcept -> std::optional<Stage>
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
                logger.error() << "stage file " << path << " is too big, the stage is empty" << std::endl;
                return Stage{};
            }

            std::ifstream file(path, std::ios::binary);
            std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            std::vector<std::string> errors;
            Stage stage = parseStage(text, errors);

            for (const auto& message : errors)
                logger.warning() << path.filename().string() << ": " << message << std::endl;
            if (stage.length == 0)
                logger.warning() << path.filename().string() << ": no length, the stage never ends" << std::endl;
            logger.info() << "loaded " << stage.waves.size() << " waves from " << path
                          << (stage.boss.empty() ? std::string() : ", boss " + stage.boss) << std::endl;
            return stage;
        }
    } catch (const std::exception& err) {
        logger.error() << "cannot load stage file " << name << ": " << err.what() << std::endl;
        return Stage{};
    }
    return std::nullopt;
}

auto RType::Game::loadStages() noexcept -> std::vector<Stage>
{
    std::vector<Stage> stages;

    for (int number = 1; auto stage = loadStage("stage-" + std::to_string(number) + ".waves"); ++number)
        stages.push_back(std::move(*stage));
    if (stages.empty())
        logger.error() << "no stage file found (assets/stages/stage-1.waves), the level stays empty" << std::endl;
    return stages;
}
