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
        source/sprites/main_dude/main_dude.cpp
        source/sprites/main_dude/main_dude.h
        source/sprites/items/bomb.cpp
        source/sprites/items/rock.cpp
        source/sprites/items/bomb.h
        source/sprites/items/rope.h
        source/sprites/non_interactive/copyrights.cpp
        source/sprites/non_interactive/copyrights.h
        source/sprites/items/rope_element.h
        source/sprites/items/rope_element.cpp
        source/sprites/items/rope.cpp
        source/sprites/moving_object.h
        source/sprites/moving_object.cpp
        source/main.cpp
        source/game_loop.cpp
        source/game_loop.h
        Makefile
        README.md source/sprites/main_dude/main_dude.cpp source/sprites/main_dude/main_dude.h source/sprites/sprite_state.h source/tiles/screen_positions.h source/globals_declarations.h source/game_loop.h source/tiles/level_generator.cpp source/tiles/level_generator.h source/tiles/map_utils.cpp source/tiles/map_utils.h source/tiles/map_tile.cpp source/tiles/map_tile.h source/collisions/collisions.cpp source/collisions/collisions.h source/hud/hud.cpp source/hud/hud.h source/sprites/sprite_info.cpp source/sprites/sprite_info.h source/camera/camera.cpp source/camera/camera.h source/memory/oam_manager.cpp source/memory/oam_manager.h source/input/input_handler.cpp source/input/input_handler.h source/sprites/spritesheet_type.h source/memory/oam_type.h source/sprites/items/rock.h source/sprites/items/jar.cpp source/sprites/items/jar.h source/sprites/enemies/snake.cpp source/sprites/enemies/snake.h source/sprites/collectibles/moniez.cpp source/sprites/collectibles/moniez.h source/tiles/map_tile_type.h source/sprites/animations/blood.cpp source/sprites/animations/blood.h source/sprites/animations/blood_element.cpp source/sprites/animations/blood_element.h source/sprites/non_interactive/spelunky_title.cpp source/sprites/non_interactive/spelunky_title.h source/tiles/tile_orientation.h source/sprites/non_interactive/title_menu_sign.cpp source/sprites/non_interactive/title_menu_sign.h source/sprites/menu_sign_type.h source/tiles/splash_screen_type.h source/rooms/left_right_rooms.h source/rooms/left_right_down_rooms.h source/rooms/left_right_up_rooms.h source/rooms/closed_rooms.h source/rooms/splash_screens.h source/rooms/entrance_rooms.h source/rooms/exit_rooms.h source/rooms/room_type.h source/sprites/enemies/bat.cpp source/sprites/enemies/bat.h source/sprites/enemies/spider.cpp source/sprites/enemies/spider.h source/sprites/sprite_type.h source/globals_definitions.h source/sound/sound_utils.h source/console/console_utils.h source/time/time_utils.h source/time/time_utils.cpp source/memory/oam_utils.cpp source/memory/oam_utils.h source/sprites/main_dude/whip.cpp source/sprites/main_dude/whip.h source/sprites/traps/spikes.cpp source/sprites/traps/spikes.h source/sprites/collectibles/chest.cpp source/sprites/collectibles/chest.h source/sprites/collectibles/crate.h source/sprites/collectibles/crate.cpp source/sprites/collectibles/shotgun.h source/sprites/collectibles/shotgun.cpp source/sprites/collectibles/bullet.h source/sprites/collectibles/bullet.cpp source/sprites/collectibles/pistol.h source/sprites/collectibles/pistol.cpp source/sprites/collectibles/spring_shoes.h source/sprites/collectibles/spring_shoes.cpp source/sprites/collectibles/mitt.h source/sprites/collectibles/mitt.cpp source/sprites/collectibles/glove.h source/sprites/collectibles/glove.cpp source/sprites/collectibles/compass.h source/sprites/collectibles/compass.cpp source/sprites/animations/got_collectible.h source/sprites/animations/got_collectible.cpp source/camera/layer_level.h source/sprites/enemies/shopkeeper.h source/sprites/enemies/shopkeeper.cpp source/sprites/main_dude/dude_utils.cpp source/sprites/main_dude/game_state.h source/sprites/main_dude/game_state.cpp source/sprites/enemies/caveman.h source/sprites/enemies/caveman.cpp)
