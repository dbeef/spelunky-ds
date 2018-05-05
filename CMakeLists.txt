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
        gfx/sprites/16x16/sprite.grit
        source/sprites/main_dude/MainDude.cpp
        source/sprites/main_dude/MainDude.h
        source/sprites/items/Bomb.cpp
        source/sprites/items/Rock.cpp
        source/sprites/items/Bomb.h
        source/sprites/items/Rope.h
        source/sprites/non_interactive/Copyrights.cpp
        source/sprites/non_interactive/Copyrights.h
        source/sprites/items/RopeElement.h
        source/sprites/items/RopeElement.cpp
        source/sprites/items/Rope.cpp
        source/console/TextManager.cpp
        source/console/TextManager.h
        source/sprites/MovingObject.h
        source/sprites/MovingObject.cpp
        source/main.cpp
        source/GameLoop.cpp
        source/tiles/BaseCaveMap.h
        source/GameLoop.h
        includes/TextBackgrounds.h
        Makefile
        README.md source/sprites/main_dude/MainDude.cpp source/sprites/main_dude/MainDude.h source/sprites/SpriteState.h source/tiles/ScreenPositions.h source/Globals.h source/GameLoop.h source/tiles/LevelGenerator.cpp source/tiles/LevelGenerator.h source/time/Timer.cpp source/time/Timer.h source/console/TextManager.cpp source/console/TextManager.h source/tiles/MapUtils.cpp source/tiles/MapUtils.h source/tiles/MapTile.cpp source/tiles/MapTile.h source/collisions/Collisions.cpp source/collisions/Collisions.h source/hud/Hud.cpp source/hud/Hud.h source/sprites/SpriteInfo.cpp source/sprites/SpriteInfo.h source/camera/Camera.cpp source/camera/Camera.h source/memory/OAMManager.cpp source/memory/OAMManager.h source/input/InputHandler.cpp source/input/InputHandler.h source/sprites/SpritesheetTypes.h source/memory/OamType.h source/sprites/items/Rock.h source/sprites/items/Jar.cpp source/sprites/items/Jar.h source/sprites/enemies/Snake.cpp source/sprites/enemies/Snake.h source/sprites/collectibles/Moniez.cpp source/sprites/collectibles/Moniez.h source/tiles/MapTileType.h source/sprites/animations/Blood.cpp source/sprites/animations/Blood.h source/sprites/animations/BloodElement.cpp source/sprites/animations/BloodElement.h source/sprites/non_interactive/SpelunkyTitle.cpp source/sprites/non_interactive/SpelunkyTitle.h source/tiles/TileOrientation.h source/sprites/non_interactive/TitleMenuSign.cpp source/sprites/non_interactive/TitleMenuSign.h source/sprites/MenuSignType.h source/tiles/SplashScreenType.h source/rooms/LeftRightRooms.h source/rooms/LeftRightDownRooms.h source/rooms/LeftRightUpRooms.h source/rooms/ClosedRooms.h source/rooms/SplashScreens.h source/rooms/EntranceRooms.h source/rooms/ExitRooms.h source/rooms/RoomType.h source/sprites/enemies/Bat.cpp source/sprites/enemies/Bat.h source/sprites/enemies/Spider.cpp source/sprites/enemies/Spider.h source/sprites/SpriteType.h)
