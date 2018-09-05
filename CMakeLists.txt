cmake_minimum_required(VERSION 3.9)
project(spelunkyds)

set(CMAKE_CXX_STANDARD 11)

include_directories(/opt/devkitpro/libnds/lib)
include_directories(/opt/devkitpro/libnds/include)
include_directories(/opt/devkitpro/libnds/include/nds)
include_directories(/opt/devkitpro/libnds/include/nds/arm9)
include_directories(/opt/devkitpro/devkitARM)
link_directories(/opt/devkitpro/libnds/lib)
link_directories(/opt/devkitpro/libnds/include)
link_directories(/opt/devkitpro/libnds/include/nds)
link_directories(/opt/devkitpro/libnds/include/nds/arm9)

add_executable(spelunkyds
        gfx/fonts/font.grit
        gfx/sprites/16x16/sprite.grit
        source/sprites/main_dude/main_dude.cpp
        source/sprites/main_dude/main_dude.h
        source/sprites/items/bomb.cpp
        source/sprites/items/rock.cpp
        source/sprites/items/arrow.cpp
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
        README.md
        source/sprites/main_dude/main_dude.cpp
        source/sprites/main_dude/main_dude.h
        source/sprites/sprite_state.hpp
        source/tiles/screen_positions.hpp
        source/globals_declarations.h
        source/game_loop.h
        source/tiles/level_generator.cpp
        source/tiles/level_generator.h
        source/tiles/map_utils.cpp
        source/tiles/map_utils.h
        source/tiles/map_tile.cpp
        source/tiles/map_tile.hpp
        source/collisions/collisions.cpp
        source/collisions/collisions.h
        source/hud/hud.cpp source/hud/hud.h
        source/sprites/sprite_info.cpp source/sprites/sprite_info.h source/camera/camera.cpp source/camera/camera.h source/memory/oam_manager.cpp source/memory/oam_manager.h source/input/input_handler.cpp source/input/input_handler.h source/sprites/spritesheet_type.hpp source/memory/oam_type.hpp source/sprites/items/rock.h source/sprites/items/jar.cpp source/sprites/items/jar.h source/sprites/enemies/snake.cpp source/sprites/enemies/snake.h source/sprites/collectibles/moniez.cpp source/sprites/collectibles/moniez.h source/tiles/map_tile_type.hpp source/sprites/animations/blood.cpp source/sprites/animations/blood.h source/sprites/animations/blood_element.cpp source/sprites/animations/blood_element.h source/sprites/non_interactive/spelunky_title.cpp source/sprites/non_interactive/spelunky_title.h source/tiles/tile_orientation.hpp source/sprites/non_interactive/title_menu_sign.cpp source/sprites/non_interactive/title_menu_sign.h source/sprites/menu_sign_type.hpp source/tiles/splash_screen_type.hpp source/rooms/left_right_rooms.hpp source/rooms/left_right_down_rooms.hpp source/rooms/left_right_up_rooms.hpp source/rooms/closed_rooms.hpp source/rooms/splash_screens.hpp source/rooms/entrance_rooms.hpp source/rooms/exit_rooms.hpp source/rooms/room_type.hpp source/sprites/enemies/bat.cpp source/sprites/enemies/bat.h source/sprites/enemies/spider.cpp source/sprites/enemies/spider.h source/sprites/sprite_type.hpp source/globals_definitions.h source/sound/sound_utils.h source/console/console_utils.h source/time/time_utils.h source/time/time_utils.cpp source/memory/oam_utils.cpp source/memory/oam_utils.h source/sprites/main_dude/whip.cpp source/sprites/main_dude/whip.h source/sprites/traps/spikes.cpp source/sprites/traps/spikes.h source/sprites/collectibles/chest.cpp source/sprites/collectibles/chest.h source/sprites/collectibles/crate.h source/sprites/collectibles/crate.cpp source/sprites/collectibles/shotgun.h source/sprites/collectibles/shotgun.cpp source/sprites/collectibles/bullet.h source/sprites/collectibles/bullet.cpp source/sprites/collectibles/pistol.h source/sprites/collectibles/pistol.cpp source/sprites/collectibles/spring_shoes.h source/sprites/collectibles/spring_shoes.cpp source/sprites/collectibles/mitt.h source/sprites/collectibles/mitt.cpp source/sprites/collectibles/glove.h source/sprites/collectibles/glove.cpp source/sprites/collectibles/compass.h source/sprites/collectibles/compass.cpp source/sprites/animations/got_collectible.h source/sprites/animations/got_collectible.cpp source/camera/layer_level.hpp source/sprites/enemies/shopkeeper.h source/sprites/enemies/shopkeeper.cpp source/sprites/main_dude/dude_utils.cpp source/sprites/main_dude/game_state.h source/sprites/main_dude/game_state.cpp source/sprites/enemies/caveman.h source/sprites/enemies/caveman.cpp source/sprites/animations/fall_poof.h source/sprites/animations/fall_poof.cpp source/sprites/items/cape.h source/sprites/items/cape.cpp source/sprites/items/spike_shoes.h source/sprites/items/spike_shoes.cpp source/sprites/items/pick.hpp source/sprites/items/pick.cpp source/sprites/items/jetpack.h source/sprites/items/jetpack.cpp source/sprites/items/parachute.hpp source/sprites/items/parachute.cpp source/sprites/items/rope_pile.hpp source/sprites/items/rope_pile.cpp source/sprites/items/bomb_bag.hpp source/sprites/items/bomb_bag.cpp source/sprites/items/bomb_box.hpp source/sprites/items/bomb_box.cpp source/sprites/animations/flame.h source/sprites/animations/flame_element.cpp source/sprites/animations/flame.cpp source/sprites/animations/flame_element.h source/sprites/enemies/damsel.h source/sprites/enemies/damsel.cpp source/sprites/animations/smooch.h source/sprites/animations/smooch.cpp source/rooms/shops.hpp source/sprites/non_interactive/lamp.h source/sprites/non_interactive/lamp.cpp source/tiles/direction.hpp source/sprites/collectibles/collectibles_utils.h source/sprites/collectibles/collectibles_utils.cpp source/sprites/shopping_object.h source/sprites/non_interactive/shopping_icon.h source/sprites/non_interactive/shopping_icon.cpp source/sprites/shopping_object.cpp source/sprites/traps/arrow_trap.h source/sprites/traps/arrow_trap.cpp source/sprites/animations/blast.h source/sprites/animations/blast.cpp source/rooms/snake_well.hpp source/sprites/enemies/skeleton.cpp source/sprites/enemies/skeleton.h source/sound/sound_utils.cpp source/sprites/animations/bone.h source/sprites/animations/bone.cpp source/sprites/animations/skull.h source/sprites/animations/skull.cpp source/sprites/animations/fake_skeleton.h source/sprites/animations/fake_skeleton.cpp source/global_definitions.cpp source/base_map.h source/sprites/sprite_utils.cpp source/sprites/sprite_utils.h)
