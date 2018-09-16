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
        source/sprites/main_dude/main_dude.hpp
        source/sprites/items/bomb.cpp
        source/sprites/items/rock.cpp
        source/sprites/items/arrow.cpp
        source/sprites/items/bomb.hpp
        source/sprites/items/rope.hpp
        source/sprites/non_interactive/copyrights.cpp
        source/sprites/non_interactive/copyrights.h
        source/sprites/items/rope_element.hpp
        source/sprites/items/rope_element.cpp
        source/sprites/items/pistol.hpp
        source/sprites/items/pistol.cpp
        source/sprites/items/rope.cpp
        source/sprites/moving_object.h
        source/sprites/moving_object.cpp
        source/main.cpp
        source/game_loop.cpp
        source/game_loop.hpp
        Makefile
        README.md
        source/sprites/main_dude/main_dude.cpp
        source/sprites/main_dude/main_dude.hpp
        source/sprites/sprite_state.hpp
        source/globals_declarations.hpp
        source/game_loop.hpp
        source/tiles/level.cpp
        source/tiles/level.hpp
        source/tiles/level_rendering_utils.cpp
        source/tiles/level_rendering_utils.hpp
        source/tiles/map_tile.cpp
        source/tiles/map_tile.hpp
        source/collisions/collisions.cpp
        source/collisions/collisions.hpp
        source/hud/hud.cpp source/hud/hud.hpp
        source/sprites/sprite_info.cpp source/sprites/sprite_info.h source/camera/camera.cpp source/camera/camera.hpp source/memory/oam_manager.cpp source/memory/oam_manager.hpp source/input/input_handler.cpp source/input/input_handler.hpp source/sprites/spritesheet_type.hpp source/memory/oam_type.hpp source/sprites/items/rock.hpp source/sprites/items/jar.cpp source/sprites/items/jar.hpp source/sprites/enemies/snake.cpp source/sprites/enemies/snake.hpp source/sprites/collectibles/moniez.cpp source/sprites/collectibles/moniez.hpp source/tiles/map_tile_type.hpp source/sprites/animations/blood.cpp source/sprites/animations/blood.hpp source/sprites/animations/blood_element.cpp source/sprites/animations/blood_element.hpp source/sprites/non_interactive/spelunky_title.cpp source/sprites/non_interactive/spelunky_title.h source/tiles/tile_orientation.hpp source/sprites/non_interactive/title_menu_sign.cpp source/sprites/non_interactive/title_menu_sign.h source/sprites/menu_sign_type.hpp source/tiles/splash_screen_type.hpp source/rooms/left_right_rooms.hpp source/rooms/left_right_down_rooms.hpp source/rooms/left_right_up_rooms.hpp source/rooms/closed_rooms.hpp source/rooms/splash_screens.hpp source/rooms/entrance_rooms.hpp source/rooms/exit_rooms.hpp source/rooms/RoomType.hpp source/sprites/enemies/bat.cpp source/sprites/enemies/bat.hpp source/sprites/enemies/spider.cpp source/sprites/enemies/spider.hpp source/sprites/sprite_type.hpp source/globals_definitions.hpp source/sound/sound_utils.hpp source/console/console_utils.hpp source/time/time_utils.h source/time/time_utils.cpp source/memory/oam_utils.cpp source/memory/oam_utils.hpp source/sprites/main_dude/whip.cpp source/sprites/main_dude/whip.hpp source/sprites/traps/spikes.cpp source/sprites/traps/spikes.h source/sprites/collectibles/chest.cpp source/sprites/collectibles/chest.hpp source/sprites/collectibles/crate.hpp source/sprites/collectibles/crate.cpp source/sprites/collectibles/shotgun.hpp source/sprites/collectibles/shotgun.cpp source/sprites/collectibles/bullet.hpp source/sprites/collectibles/bullet.cpp source/sprites/collectibles/spring_shoes.hpp source/sprites/collectibles/spring_shoes.cpp source/sprites/collectibles/mitt.hpp source/sprites/collectibles/mitt.cpp source/sprites/collectibles/glove.hpp source/sprites/collectibles/glove.cpp source/sprites/collectibles/compass.hpp source/sprites/collectibles/compass.cpp source/sprites/animations/got_collectible.hpp source/sprites/animations/got_collectible.cpp source/camera/layer_level.hpp source/sprites/enemies/shopkeeper.hpp source/sprites/enemies/shopkeeper.cpp source/sprites/main_dude/dude_utils.cpp source/sprites/main_dude/game_state.hpp source/sprites/main_dude/game_state.cpp source/sprites/enemies/caveman.hpp source/sprites/enemies/caveman.cpp source/sprites/animations/fall_poof.hpp source/sprites/animations/fall_poof.cpp source/sprites/items/cape.hpp source/sprites/items/cape.cpp source/sprites/items/spike_shoes.hpp source/sprites/items/spike_shoes.cpp source/sprites/items/pick.hpp source/sprites/items/pick.cpp source/sprites/items/jetpack.hpp source/sprites/items/jetpack.cpp source/sprites/items/parachute.hpp source/sprites/items/parachute.cpp source/sprites/items/rope_pile.hpp source/sprites/items/rope_pile.cpp source/sprites/items/bomb_bag.hpp source/sprites/items/bomb_bag.cpp source/sprites/items/bomb_box.hpp source/sprites/items/bomb_box.cpp source/sprites/animations/flame.hpp source/sprites/animations/flame_element.cpp source/sprites/animations/flame.cpp source/sprites/animations/flame_element.hpp source/sprites/enemies/damsel.hpp source/sprites/enemies/damsel.cpp source/sprites/animations/smooch.hpp source/sprites/animations/smooch.cpp source/rooms/shops.hpp source/sprites/non_interactive/lamp.h source/sprites/non_interactive/lamp.cpp source/tiles/direction.hpp source/sprites/collectibles/collectibles_utils.hpp source/sprites/collectibles/collectibles_utils.cpp source/sprites/shopping_object.h source/sprites/non_interactive/shopping_icon.h source/sprites/non_interactive/shopping_icon.cpp source/sprites/shopping_object.cpp source/sprites/traps/arrow_trap.h source/sprites/traps/arrow_trap.cpp source/sprites/animations/blast.hpp source/sprites/animations/blast.cpp source/rooms/snake_well.hpp source/sprites/enemies/skeleton.cpp source/sprites/enemies/skeleton.hpp source/sound/sound_utils.cpp source/sprites/animations/bone.hpp source/sprites/animations/bone.cpp source/sprites/animations/skull.hpp source/sprites/animations/skull.cpp source/sprites/animations/fake_skeleton.hpp source/sprites/animations/fake_skeleton.cpp source/globals_definitions.cpp source/base_map.hpp source/sprites/sprite_utils.cpp source/sprites/sprite_utils.hpp source/tiles/level_generator.hpp source/tiles/level_generator.cpp source/sprites/simple_sprite_wrapper.h source/sprites/simple_sprite_wrapper.cpp source/tiles/populating_utils.hpp source/tiles/populating_utils.cpp)
