cmake_minimum_required(VERSION 3.9)
project(spelunkyds)

set(CMAKE_CXX_STANDARD 11)

include_directories(~/devkitPro/libnds/lib)
include_directories(~/devkitPro/libnds/include)
include_directories(~/devkitPro/libnds/include/nds)
include_directories(~/devkitPro/libnds/include/nds/arm9)
include_directories(~/devkitPro/devkitARM)
link_directories(~/devkitPro/libnds/lib)
link_directories(~/devkitPro/libnds/include)
link_directories(~/devkitPro/libnds/include/nds)
link_directories(~/devkitPro/libnds/include/nds/arm9)

add_executable(spelunkyds
        gfx/fonts/font.grit
        scratches/font.png
        gfx/sprites/sprite.grit
        source/animations/MainDude.cpp
        source/animations/MainDude.h
        source/animations/Bomb.cpp
        source/animations/Rock.cpp
        source/animations/Bomb.h
        source/animations/Rope.h
        source/animations/Copyrights.cpp
        source/animations/Copyrights.h
        source/animations/RopeElement.h
        source/animations/RopeElement.cpp
        source/animations/Rope.cpp
        source/console/TextManager.cpp
        source/console/TextManager.h
        source/sprites/MovingObject.h
        source/sprites/MovingObject.cpp
        source/main.cpp
        source/level_layout/GameLoop.cpp
        source/level_layout/BaseCaveMap.h
        source/level_layout/GameLoop.h
        includes/TextBackgrounds.h
        Makefile
        README.md source/animations/MainDude.cpp source/animations/MainDude.h source/animations/SpriteState.h source/level_layout/ScreenPositions.h source/Globals.h source/level_layout/GameLoop.h source/level_layout/LevelGenerator.cpp source/level_layout/LevelGenerator.h source/time/Timer.cpp source/time/Timer.h source/console/TextManager.cpp source/console/TextManager.h source/level_layout/MapUtils.cpp source/level_layout/MapUtils.h source/level_layout/MapTile.cpp source/level_layout/MapTile.h source/animations/Collisions.cpp source/animations/Collisions.h source/hud/Hud.cpp source/hud/Hud.h source/level_layout/SpriteInfo.cpp source/level_layout/SpriteInfo.h source/animations/Camera.cpp source/animations/Camera.h source/input/touchUtils.h source/animations/OAMManager.cpp source/animations/OAMManager.h source/input/InputHandler.cpp source/input/InputHandler.h source/sprites/SpriteTypes.h source/animations/OamType.h source/animations/Rock.h source/animations/Jar.cpp source/animations/Jar.h source/animations/Snake.cpp source/animations/Snake.h source/animations/Moniez.cpp source/animations/Moniez.h source/level_layout/MapTileType.h source/animations/Blood.cpp source/animations/Blood.h source/animations/BloodElement.cpp source/animations/BloodElement.h source/animations/SpelunkyTitle.cpp source/animations/SpelunkyTitle.h source/level_layout/TileOrientation.h source/animations/TitleMenuSign.cpp source/animations/TitleMenuSign.h source/animations/MenuSignType.h source/level_layout/SplashScreenType.h source/level_layout/rooms/LeftRightRooms.h source/level_layout/rooms/LeftRightDownRooms.h source/level_layout/rooms/LeftRightUpRooms.h source/level_layout/rooms/ClosedRooms.h source/level_layout/rooms/SplashScreens.h source/level_layout/rooms/EntranceRooms.h source/level_layout/rooms/ExitRooms.h source/level_layout/rooms/RoomType.h source/animations/Bat.cpp source/animations/Bat.h)
