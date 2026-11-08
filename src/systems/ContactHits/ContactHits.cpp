/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** ContactHits system
*/

#include "ContactHits.hpp"
#include "../../game/Damage/Damage.hpp"
#include <rtype/game.hpp>
#include <algorithm>
#include <chrono>
#include <vector>


RType::Systems::ContactHits::ContactHits() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

void RType::Systems::ContactHits::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& contact = engine.get<Components::ContactDamage>()[entity];
    const auto& body = engine.get<Components::Body>()[entity];
    const auto& position = engine.get<Protecs::Components::Transform>()[entity].position;
    std::vector<Wrecks::ECS::Entity> touched;

    contact.cooldown = std::max(0.0F, contact.cooldown - std::chrono::duration<float>(dt).count());
    if (contact.cooldown > 0.0F || m_Requests->destroying(entity))
        return;

    for (auto [other, otherBody] : engine.get<Components::Body>())
        if (engine.hasComponent<Components::Enemy>(other) && !engine.hasComponent<Components::Armored>(other) && !m_Requests->destroying(other)
            && Rules::overlap(position + body.offset, body.size, engine.get<Protecs::Components::Transform>()[other].position + otherBody.offset, otherBody.size))
            touched.push_back(other);

    for (auto enemy : touched) {
        auto reward = engine.get<Components::Enemy>()[enemy].reward;

        if (Game::damage(engine, *m_Requests, enemy, contact.damage) && engine.hasComponent<Protecs::Components::PlayerStats>(body.owner))
            engine.get<Protecs::Components::PlayerStats>()[body.owner].score += reward;
    }
    if (!touched.empty())
        contact.cooldown = contact.interval;
}
