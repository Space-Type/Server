##
## EPITECH PROJECT, 2026
## Project - RType
## File description:
## Main project build
## file
##

cmake_minimum_required(VERSION 3.15)

project(RType-Server VERSION 1.0
                     DESCRIPTION "R-Type Game Server - R-Type project"
                     HOMEPAGE_URL "https://github.com/Space-Type/Server"
                     LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)


# ----------- Project Dependencies ----------- #

if (NOT TARGET wrecks)
    add_subdirectory(lib/wrecks)
endif()
if (NOT TARGET protecs)
    add_subdirectory(lib/protecs)
endif()


# --------------- Project Build -------------- #

set(SOURCES
    src/components/AimedBursts/AimedBursts.hpp
    src/components/Armored/Armored.hpp
    src/components/Attached/Attached.hpp
    src/components/Body/Body.hpp
    src/components/Boss/Boss.hpp
    src/components/ContactDamage/ContactDamage.hpp
    src/components/Enemy/Enemy.hpp
    src/components/Enrage/Enrage.hpp
    src/components/Force/Force.hpp
    src/components/InputBuffer/InputBuffer.cpp
    src/components/LevelState/LevelState.hpp
    src/components/Loot/Loot.hpp
    src/components/InputBuffer/InputBuffer.hpp
    src/components/NetworkID/NetworkID.cpp
    src/components/NetworkID/NetworkID.hpp
    src/components/Pickup/Pickup.hpp
    src/components/Player/Player.cpp
    src/components/Player/Player.hpp
    src/components/RandomShots/RandomShots.hpp
    src/components/Shield/Shield.hpp
    src/components/StopAt/StopAt.hpp
    src/components/Swing/Swing.hpp
    src/components/Manager.hpp
    src/game/Blueprints/Blueprints.cpp
    src/game/Blueprints/Blueprints.hpp
    src/game/Damage/Damage.cpp
    src/game/Damage/Damage.hpp
    src/game/DevOptions/DevOptions.cpp
    src/game/DevOptions/DevOptions.hpp
    src/game/Game/Game.cpp
    src/game/Game/Game.hpp
    src/game/Game/Level.cpp
    src/game/Requests/Requests.cpp
    src/game/Requests/Requests.hpp
    src/game/Rules.hpp
    src/game/Waves/Waves.cpp
    src/game/Waves/Waves.hpp
    src/game/Server/Server.cpp
    src/game/Server/Server.hpp
    src/game/Engine.hpp
    src/systems/Attachment/Attachment.cpp
    src/systems/Attachment/Attachment.hpp
    src/systems/BossDefeat/BossDefeat.cpp
    src/systems/BossDefeat/BossDefeat.hpp
    src/systems/BufferedInput/BufferedInput.cpp
    src/systems/BurstFire/BurstFire.cpp
    src/systems/BurstFire/BurstFire.hpp
    src/systems/Collecting/Collecting.cpp
    src/systems/Collecting/Collecting.hpp
    src/systems/Collisions/Collisions.cpp
    src/systems/Collisions/Collisions.hpp
    src/systems/ContactHits/ContactHits.cpp
    src/systems/ContactHits/ContactHits.hpp
    src/systems/Flight/Flight.cpp
    src/systems/Looting/Looting.cpp
    src/systems/Looting/Looting.hpp
    src/systems/PhaseChange/PhaseChange.cpp
    src/systems/PhaseChange/PhaseChange.hpp
    src/systems/Flight/Flight.hpp
    src/systems/ForceControl/ForceControl.cpp
    src/systems/ForceControl/ForceControl.hpp
    src/systems/ForceFire/ForceFire.cpp
    src/systems/ForceFire/ForceFire.hpp
    src/systems/Scrolling/Scrolling.cpp
    src/systems/Scrolling/Scrolling.hpp
    src/systems/Shielding/Shielding.cpp
    src/systems/Shielding/Shielding.hpp
    src/systems/Shooting/Shooting.cpp
    src/systems/Shooting/Shooting.hpp
    src/systems/StageSpawner/StageSpawner.cpp
    src/systems/StageSpawner/StageSpawner.hpp
    src/systems/StageTransition/StageTransition.cpp
    src/systems/StageTransition/StageTransition.hpp
    src/systems/Swinging/Swinging.cpp
    src/systems/Swinging/Swinging.hpp
    src/systems/BufferedInput/BufferedInput.hpp
    src/systems/Tuple.hpp
    src/utils/Logger/Logger.cpp
    src/utils/Logger/Logger.hpp
    src/utils/SignalHandler.hpp
    src/main.cpp)

add_executable(r-type_server ${SOURCES})

# windows.h (included by the Wrecks and Protecs networking headers) defines
# ERROR (clashes with Logger::ERROR) and min / max macros (std::min, std::max)
if (WIN32)
    target_compile_definitions(r-type_server PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX NOGDI)
endif()

target_include_directories(r-type_server PUBLIC include
                                         PUBLIC lib/wrecks/include
                                         PUBLIC lib/protecs/include)

target_link_libraries(r-type_server PUBLIC wrecks
                                    PUBLIC protecs)
