/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** ForceControl system
*/

#include "ForceControl.hpp"
#include <rtype/game.hpp>
#include <algorithm>
#include <chrono>
#include <cmath>


RType::Systems::ForceControl::ForceControl() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

void RType::Systems::ForceControl::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    using State = Components::Force::State;
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& force = engine.get<Components::Force>()[entity];
    const auto& body = engine.get<Components::Body>()[entity];
    auto& position = engine.get<Protecs::Components::Transform>()[entity].position;
    auto owner = body.owner;

    if (m_Requests->destroying(entity))
        return;
    if (!engine.hasComponent<Protecs::Components::PlayerStats>(owner) || !engine.hasComponent<Components::Body>(owner)
        || engine.get<Components::Body>()[owner].out || engine.get<Protecs::Components::PlayerStats>()[owner].lives < force.lives) {
        m_Requests->destroy(entity);
        return;
    }

    const auto& shipBody = engine.get<Components::Body>()[owner];
    Rules::Vec2D ship = engine.get<Protecs::Components::Transform>()[owner].position + shipBody.offset;
    bool held = engine.hasComponent<Protecs::Components::PlayerInput>(owner) && engine.get<Protecs::Components::PlayerInput>()[owner].force;
    bool pressed = held && !force.pressed;
    float seconds = std::chrono::duration<float>(dt).count();

    force.pressed = held;
    if (pressed && force.state == State::Attached) {
        force.state = State::Launched;
        force.travelled = 0.0F;
    } else if (pressed && (force.state == State::Launched || force.state == State::Floating)) {
        force.state = State::Returning;
    }

    if (force.state == State::Attached) {
        position = Rules::forcePosition(ship, shipBody.size, body.size, force.side == Components::Force::Side::Front) - body.offset;
        return;
    }
    if (force.state == State::Launched) {
        float step = Rules::FORCE_LAUNCH_SPEED * seconds;
        float x = position.x + (force.side == Components::Force::Side::Front ? step : -step);
        float clamped = std::clamp(x, 0.0F, Protecs::MAP_MAX_X - body.size.x);

        position.x = clamped;
        force.travelled += step;
        if (clamped != x || force.travelled >= Rules::FORCE_LAUNCH_DISTANCE)
            force.state = State::Floating;
    } else if (force.state == State::Returning) {
        Rules::Vec2D target = ship + shipBody.size / 2.0F - body.size / 2.0F - body.offset;
        Rules::Vec2D way = target - position;
        float distance = std::sqrt(way.x * way.x + way.y * way.y);
        float step = Rules::FORCE_RETURN_SPEED * seconds;

        position = distance <= step ? target : position + way * (step / distance);
    }

    // Launched, floating or returning: touching its ship, it attaches on the side it touches
    if (Rules::overlap(position + body.offset, body.size, ship, shipBody.size)) {
        bool front = position.x + body.offset.x + body.size.x / 2.0F >= ship.x + shipBody.size.x / 2.0F;

        force.side = front ? Components::Force::Side::Front : Components::Force::Side::Back;
        force.state = State::Attached;
        position = Rules::forcePosition(ship, shipBody.size, body.size, front) - body.offset;
    }
}
