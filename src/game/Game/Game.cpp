/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Game class
*/

#include "Game.hpp"
#include <chrono>
#include <rtype/game.hpp>
#include <rtype/utils.hpp>
#include <tuple>
#include <variant>


namespace Packets = Protecs::Networking::Packets;


RType::Game::Game::Game() noexcept
    : m_Engine()
    , m_FixedUpdateElapsedTime(0)
    , m_NetworkUpdateElapsedTime(0)
    , m_NetworkUpdates(0)
    , m_SimulationTick(0)
    , m_Players()
    , m_IncomingEvents()
    , m_OutgoingEvents()
    , m_PullEvents()
{
    m_Engine.get<Systems::BufferedInput>().setSimulationTick(m_SimulationTick);
}

/**
 * Runs a single frame of
 * game logic.
 */
void RType::Game::Game::update(Wrecks::DeltaTime dt)
{
    m_FixedUpdateElapsedTime   += dt;
    m_NetworkUpdateElapsedTime += dt;

    handleEvents(0);

    while (m_FixedUpdateElapsedTime >= Protecs::FIXED_UPDATE_RATE) {
        handleEvents(m_SimulationTick);
        m_Engine.fixedUpdate(Protecs::FIXED_UPDATE_RATE);
        m_SimulationTick++;
        m_FixedUpdateElapsedTime -= Protecs::FIXED_UPDATE_RATE;
    }

    updateGameplay(dt);

    m_Engine.update(dt);

    if (m_NetworkUpdateElapsedTime >= NETWORK_UPDATE_RATE) {
        updateNetwork();
        m_NetworkUpdateElapsedTime = m_NetworkUpdateElapsedTime.zero();
    }
}

/**
 * Pushes an external event into
 * the game's external event queue.
 *
 * The events will be handled on the
 * next update().
 */
bool RType::Game::Game::pushEvent(const Protecs::Networking::Address& sender, Packets::PacketData&& data)
{
    if (std::holds_alternative<Packets::PlayerJoin>(data)) {
        auto& request = std::get<Packets::PlayerJoin>(data);
        if (!registerPlayer(sender, request.str()))
            return false;
    }

auto playerIt = m_Players.find(sender);
if (playerIt == m_Players.end()) {
    logger.warning() << "rejected request from " << sender << " (not a player)" << std::endl;
    return false;
}

auto& player = m_Engine.get<Components::Player>()[playerIt->second];

m_IncomingEvents.emplace_back(&player, std::move(data));
return true;
}

/**
* Pops an internal event from
* the game's internal event queue.
*
* Events may be pushed onto this
* queue by the Game on update().
 */
bool RType::Game::Game::popEvent(Event& data)
{
    if (m_OutgoingEvents.empty())
        return false;

    data = std::move(m_OutgoingEvents.front());
    m_OutgoingEvents.pop_front();

    return true;
}

/**
 * Registers a new player in
 * the game. Returns true if
 * successful, and false if not.
 */
bool RType::Game::Game::registerPlayer(const Protecs::Networking::Address& address, std::string_view name)
{
    auto playerIt = m_Players.find(address);
    if (playerIt != m_Players.end()) {
        logger.warning() << "rejected request to join from " << address << " (already a player)" << std::endl;
        return false;
    }

    auto playerEntity = m_Engine.createEntity();
    m_Players[address] = playerEntity;

    m_Engine.addComponent(playerEntity, Components::Player{ address, std::string(name) });
    m_Engine.addComponent(playerEntity, Components::NetworkID::fromEntity(playerEntity));

    logger.info() << "player \"" << name << "\" (" << address << ") joined the game" << std::endl;
    return true;
}

/**
 * Disconnects a player from
 * the current game (if possible).
 */
void RType::Game::Game::disconnectPlayer(const Protecs::Networking::Address& address)
{
    auto playerIt = m_Players.find(address);
    if (playerIt == m_Players.end()) {
        logger.warning() << "attempt to disconnect unknown player of address " << address << std::endl;
        return;
    }

    auto& player = m_Engine.get<Components::Player>()[playerIt->second];
    auto networkId = m_Engine.get<Components::NetworkID>()[playerIt->second].id;

    logger.info() << "player \"" << player.name << "\" (" << player.address << ") disconnected" << std::endl;

    m_Engine.deleteEntity(playerIt->second);
    m_Players.erase(playerIt);

    broadcastPacket(Packets::EntityDelete{ .id = networkId });
}

/**
 * Handles any and all external
 * events queued by the server.
 */
void RType::Game::Game::handleEvents(Protecs::Tick tick)
{
    m_PullEvents(tick);

    while (!m_IncomingEvents.empty()) {
        auto& [player, data] = m_IncomingEvents.front();
        if (!player)
            continue;

        std::visit(EventHandler(*this, *player), data);

        m_IncomingEvents.pop_front();
    }
}

/**
 * Sends entity component data
 * to all connected players.
 */
void RType::Game::Game::updateNetwork()
{
    m_NetworkUpdates++;
    constexpr std::size_t protecsComponentCount = Wrecks::Utils::container_elements_v<Protecs::Components::Manager>;

    for (std::size_t componentIndex = 0; componentIndex < protecsComponentCount; componentIndex++) {
        Wrecks::Utils::visitType<Protecs::Components::Variant>([&](auto type){
            using Component = decltype(type)::type;

            if constexpr (std::is_same_v<Component, Protecs::Components::PlayerInput>)
                return;

            auto& componentData = m_Engine.get<Component>();

            for (auto [entity, data] : componentData) {
                auto networkEntity = m_Engine.get<Components::NetworkID>()[entity].id;
                broadcastPacket(Packets::EntityComponentData{
                    .simulationTick = m_SimulationTick,
                    .entity = networkEntity,
                    .component = data,
                });
            }
        }, componentIndex);
    }

    if (m_NetworkUpdates >= UPDATES_PER_INPUT_SYNC) {
        broadcastPacket(Packets::PlayerInput{});
        m_NetworkUpdates = 0;
    }
}

/**
 * Adds the necessary components to
 * the given entity for it to qualify
 * as a player.
 */
void RType::Game::Game::makePlayer(Wrecks::ECS::Entity entity)
{
    addSharedComponent(entity, Protecs::Components::Transform{ .position = {}, .rotation = 0.0, .scale = 1.0 });
    addSharedComponent(entity, Protecs::Components::Velocity{});
    addSharedComponent(entity, Protecs::Components::Movement{ {}, 1.f });
    addSharedComponent(entity, Protecs::Components::PlayerInput{});
    addSharedComponent(entity, Protecs::Components::PlayerName::fromStr(m_Engine.get<Components::Player>()[entity].name));
    m_Engine.addComponent(entity, Components::InputBuffer{});
    initPlayerGameplay(entity);
}

/**
 * Retrieves a local ECS entity
 * from a given entity network ID.
 */
auto RType::Game::Game::findEntity(Protecs::NetworkEntity network) const -> Wrecks::ECS::Entity
{
    for (auto [entity, data] : m_Engine.get<Components::NetworkID>()) {
        if (data.id == network)
            return entity;
    }

    return UINT32_MAX;
}

/**
 * Broadcasts a packet to
 * all connected players.
 */
void RType::Game::Game::broadcastPacket(Packets::PacketData&& data)
{
    for (const auto& [_, playerEntity] : m_Players) {
        const auto& player = m_Engine.get<Components::Player>()[playerEntity];
        m_OutgoingEvents.emplace_back(&player, data);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//                                    Event Handler                                    //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////

RType::Game::Game::EventHandler::EventHandler(Game& game, Components::Player& player) noexcept
    : m_Game(game)
    , m_Player(player)
{
}

void RType::Game::Game::EventHandler::operator()(const Packets::EntityComponentData& data)
{
    logger.debug() << "handling " << data.packetName() << " for " << m_Player.name << std::endl;
}

void RType::Game::Game::EventHandler::operator()(const Packets::EntityCreate& data)
{
    logger.debug() << "handling " << data.packetName() << " for " << m_Player.name << std::endl;
}

void RType::Game::Game::EventHandler::operator()(const Packets::EntityDelete& data)
{
    logger.debug() << "handling " << data.packetName() << " from " << m_Player.name << std::endl;
}

/**
 * Entities that fit in one EntityList packet: the packet buffer
 * (Packet::raw) holds the packet type, the list header and
 * a few (network id, signature) pairs. Clients handle every
 * EntityList packet on its own.
 */
static constexpr std::size_t ENTITY_LIST_MAX =
    (std::tuple_size_v<decltype(Protecs::Networking::Packet::raw)> - 1 - Packets::EntityList::partialPackedSize())
    / (sizeof(Protecs::NetworkEntity) + sizeof(Protecs::Signature));

static_assert(ENTITY_LIST_MAX > 0, "an EntityList packet must hold at least one entity");

void RType::Game::Game::EventHandler::operator()(const Packets::EntityList& data)
{
    logger.debug() << "handling " << data.packetName() << " for " << m_Player.name << std::endl;

    Packets::EntityList entityList = { .simulationTick = m_Game.m_SimulationTick, .entities = {} };

    entityList.entities.reserve(m_Game.m_Engine.entities().size());
    for (auto [entity, serverSig] : m_Game.m_Engine.entities()) {
        // Deleted entities keep their slot (empty signature) until it is reused
        if (!m_Game.m_Engine.hasComponent<Components::NetworkID>(entity))
            continue;

        const auto signature = serverSig & Protecs::Components::Manager::mask();  // remove server-side components
        auto& network = m_Game.m_Engine.get<Components::NetworkID>()[entity];

        entityList.entities.emplace_back(network.id, signature);

        // A packet only holds a few entities: send the list in several of them
        if (entityList.entities.size() == ENTITY_LIST_MAX) {
            m_Game.m_OutgoingEvents.emplace_back(&m_Player, entityList);
            entityList.entities.clear();
        }
    }

    if (!entityList.entities.empty())
        m_Game.m_OutgoingEvents.emplace_back(&m_Player, std::move(entityList));
}


void RType::Game::Game::EventHandler::operator()(const Packets::EntityRemoveComponent& data)
{
    logger.debug() << "handling " << data.packetName() << " for " << m_Player.name << std::endl;
}

void RType::Game::Game::EventHandler::operator()(const Packets::EntitySyncComponents& data)
{
    logger.debug() << "handling " << data.packetName() << " for " << m_Player.name << std::endl;

    auto entity = m_Game.findEntity(data.entity);
    if (entity == UINT32_MAX) {
        logger.warning() << "received unknown network ID " << data.entity << std::endl;
        // TODO: send error packet
        return;
    }

    // Note: when synchronizing components, only shared
    // components are considered. The client may send
    // extra components in the entity's signature, but
    // those are completely ignored.
    auto signature = m_Game.m_Engine.entitySignature(entity) & Protecs::Components::Manager::mask();

    m_Game.m_OutgoingEvents.emplace_back(&m_Player, Packets::EntitySyncComponents{
        .entity    = data.entity,
        .signature = signature,
    });

    for (std::size_t i = 0; i < Wrecks::Utils::container_elements_v<Protecs::Components::Manager>; i++) {
        if (signature.test(i) == data.signature.test(i))
            continue;

        bool componentIsMissing = signature.test(i) && !data.signature.test(i);

        if (componentIsMissing) {
            // component missing, add it
            Wrecks::Utils::visitType<Protecs::Components::Variant>([&](auto type){
                using Component = decltype(type)::type;
                m_Game.m_OutgoingEvents.emplace_back(&m_Player, Packets::EntityComponentData{
                    .entity = data.entity,
                    .component = m_Game.m_Engine.get<Component>()[entity],
                });
            }, i);
        } else {
            // component superfluous, remove it
            m_Game.m_OutgoingEvents.emplace_back(&m_Player, Packets::EntityRemoveComponent{
                .entity = data.entity,
                .type = static_cast<Protecs::ComponentType>(i),
            });
        }
    }
}

void RType::Game::Game::EventHandler::operator()(const Packets::Ping& data)
{
    logger.debug() << "handling " << data.packetName() << " for " << m_Player.name << std::endl;
}

void RType::Game::Game::EventHandler::operator()(const Packets::PlayerId& data)
{
    logger.debug() << "handling " << data.packetName() << " for " << m_Player.name << std::endl;

    auto playerEntity = m_Game.m_Players.at(m_Player.address);
    auto networkId = m_Game.m_Engine.get<Components::NetworkID>()[playerEntity].id;

    // Ignore data contents.
    // If a player sends a PlayerId packet,
    // send them their Network ID, so that
    // they can retrieve it in case they lost it.
    m_Game.m_OutgoingEvents.emplace_back(&m_Player, Packets::PlayerId{ .id = networkId });
}

void RType::Game::Game::EventHandler::operator()(const Packets::PlayerInput& data)
{
    logger.debug() << "handling " << data.packetName() << " for " << m_Player.name << std::endl;

    if (data.simulationTick < m_Game.m_SimulationTick) {
        logger.warning() << "ignoring input for past tick from " << m_Player.name << std::endl;
        return;
    }

    auto playerEntity = m_Game.m_Players.at(m_Player.address);
    auto& inputs = m_Game.m_Engine.get<Components::InputBuffer>()[playerEntity];

    inputs.push(data);
}

void RType::Game::Game::EventHandler::operator()(const Packets::PlayerJoin& data)
{
    logger.debug() << "handling " << data.packetName() << " for " << m_Player.name << std::endl;

    auto playerEntity = m_Game.m_Players.at(m_Player.address);
    auto networkId = m_Game.m_Engine.get<Components::NetworkID>()[playerEntity].id;

    m_Game.m_OutgoingEvents.emplace_back(&m_Player, Packets::PlayerId{ networkId });

    // send simulation tick sync to player
    operator()(Packets::TickSync{ .clientTick = 0 });

    m_Game.broadcastPacket(Packets::EntityCreate{ .simulationTick = m_Game.m_SimulationTick, .id = networkId, .type = Packets::EntityCreate::PLAYER });
    m_Game.makePlayer(playerEntity);

    // send entity list to player
    operator()(Packets::EntityList{});
}

void RType::Game::Game::EventHandler::operator()(const Packets::PlayerLeave& data)
{
    logger.debug() << "handling " << data.packetName() << " for " << m_Player.name << std::endl;

    m_Game.disconnectPlayer(m_Player.address);
}

void RType::Game::Game::EventHandler::operator()(const Packets::TickSync& data)
{
    // Don't log since it's too much, even for debug
    m_Game.m_OutgoingEvents.emplace_back(&m_Player, Packets::TickSync{
        .clientTick = data.clientTick,
        .serverTick = m_Game.m_SimulationTick,
        .clientTime = data.clientTime,
        .serverTime = std::chrono::steady_clock::now(),
    });
}
