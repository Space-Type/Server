/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Collecting system
*/

#include "Collecting.hpp"
#include <rtype/game.hpp>
#include <algorithm>
#include <stdexcept>
#include <variant>


RType::Systems::Collecting::Collecting() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

/**
 * The first player or player shot touching the pickup
 * takes it.
 */
void RType::Systems::Collecting::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& body = engine.get<Components::Body>()[entity];
    const auto& position = engine.get<Protecs::Components::Transform>()[entity].position;

    if (m_Requests->destroying(entity))
        return;

    for (auto [other, otherBody] : engine.get<Components::Body>()) {
        bool shot = otherBody.type == Rules::EntityCreate::PLAYER_MISSILE || Rules::isBeam(otherBody.type);
        auto player = shot ? otherBody.owner : other;

        if ((!shot && otherBody.type != Rules::EntityCreate::PLAYER) || m_Requests->destroying(other)
            || !Rules::overlap(position + body.offset, body.size,
                               engine.get<Protecs::Components::Transform>()[other].position + otherBody.offset, otherBody.size))
            continue;
        if (!engine.hasComponent<Protecs::Components::PlayerStats>(player) || engine.get<Components::Body>()[player].out)
            continue;

        give(engine.get<Components::Pickup>()[entity].item, player);
        if (otherBody.type == Rules::EntityCreate::PLAYER_MISSILE)
            m_Requests->destroy(other);
        m_Requests->destroy(entity);
        return;
    }
}

/**
 * Clients see the change through PlayerStats. A life over
 * Rules::MAX_LIVES is lost: the pickup is taken anyway.
 */
void RType::Systems::Collecting::give(Components::Item item, Wrecks::ECS::Entity player)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& stats = engine.get<Protecs::Components::PlayerStats>()[player];

    switch (item) {
        case Components::Item::Life:
            stats.lives = std::min<std::uint8_t>(static_cast<std::uint8_t>(stats.lives + 1), Rules::MAX_LIVES);
            break;
        case Components::Item::Force:
            giveForce(player);
            break;
    }
}

namespace {
    // A Force of `level`, as `state` (position: its top-left corner); hurts on contact and blocks enemy missiles
    RType::Game::SpawnRequest forceRequest(Wrecks::ECS::Entity player, RType::Components::Force state, RType::Rules::Vec2D position)
    {
        return {
            .type = RType::Rules::forceType(state.level),
            .position = position,
            .size = RType::Rules::FORCE_SIZES[state.level - 1],
            .owner = player,
            .components = {
                state,
                RType::Components::ContactDamage{ .damage = state.level, .interval = RType::Rules::FORCE_HIT_INTERVAL },
                RType::Components::Shield{},
            },
        };
    }

    // The Force of a spawn request (one is always there in forceRequest)
    RType::Components::Force& forceOf(RType::Game::SpawnRequest& request)
    {
        for (auto& component : request.components)
            if (auto* force = std::get_if<RType::Components::Force>(&component))
                return *force;
        throw std::logic_error("a Force spawn request without its Force");
    }
}

/**
 * No Force: a level 1 one, attached in front. A Force (or
 * one taken earlier in this step): one more level, so it
 * is created again with the bigger sprite and hitbox, as
 * it was (attached or flying). At the last level: points.
 */
void RType::Systems::Collecting::giveForce(Wrecks::ECS::Entity player)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& stats = engine.get<Protecs::Components::PlayerStats>()[player];

    for (auto& request : m_Requests->spawns) {
        if (!Rules::isForce(request.type) || request.owner != player)
            continue;

        auto state = forceOf(request);

        if (state.level >= Rules::FORCE_LEVELS) {
            stats.score += Rules::FORCE_MAX_SCORE;
            return;
        }
        ++state.level;
        request = forceRequest(player, state, request.position);
        return;
    }
    for (auto [entity, force] : engine.get<Components::Force>()) {
        if (engine.get<Components::Body>()[entity].owner != player || m_Requests->destroying(entity))
            continue;
        if (force.level >= Rules::FORCE_LEVELS) {
            stats.score += Rules::FORCE_MAX_SCORE;
            return;
        }

        auto state = force;

        ++state.level;
        m_Requests->destroy(entity);
        m_Requests->spawns.push_back(forceRequest(player, state, engine.get<Protecs::Components::Transform>()[entity].position));
        return;
    }

    const auto& ship = engine.get<Components::Body>()[player];
    Rules::Vec2D shipPosition = engine.get<Protecs::Components::Transform>()[player].position + ship.offset;

    m_Requests->spawns.push_back(forceRequest(player, Components::Force{ .level = 1, .lives = stats.lives },
                                              Rules::forcePosition(shipPosition, ship.size, Rules::FORCE_SIZES[0], true)));
}
