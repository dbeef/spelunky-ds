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
        source/console/TextManager.cpp
        source/console/TextManager.h
        source/main.cpp
        source/level_layout/Scrolling.cpp
        source/level_layout/Scrolling.h
        includes/TextBackgrounds.h
        Makefile
        README.md source/animations/MainDude.cpp source/animations/MainDude.h source/animations/SpriteState.h source/level_layout/ScreenPositions.h source/Consts.h source/level_layout/Scrolling.h source/level_layout/LevelGenerator.cpp source/level_layout/LevelGenerator.h source/time/Timer.cpp source/time/Timer.h source/console/TextManager.cpp source/console/TextManager.h source/level_layout/MapUtils.cpp source/level_layout/MapUtils.h source/level_layout/MapTile.cpp source/level_layout/MapTile.h source/animations/Collisions.cpp source/animations/Collisions.h source/animations/Snake.cpp source/animations/Snake.h)
