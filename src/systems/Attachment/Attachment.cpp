/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Attachment system
*/

#include "Attachment.hpp"
#include <rtype/game.hpp>


RType::Systems::Attachment::Attachment() noexcept
    : m_Engine(nullptr)
    , m_Requests(nullptr)
{

}

void RType::Systems::Attachment::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    auto& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    const auto& attached = engine.get<Components::Attached>()[entity];

    if (!engine.hasComponent<Components::Body>(attached.parent) || m_Requests->destroying(attached.parent)) {
        m_Requests->destroy(entity);
        return;
    }
    engine.get<Protecs::Components::Transform>()[entity].position
        = engine.get<Protecs::Components::Transform>()[attached.parent].position + attached.offset;
}
