cmake_minimum_required(VERSION 3.9)
project(spelunkyds)

set(CMAKE_CXX_STANDARD 14)

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
        source/decorations/_base_decoration.cpp
        source/decorations/_base_decoration.h
        source/creatures/main_dude/main_dude.cpp
        source/creatures/main_dude/main_dude.hpp
        source/creatures/items/bomb.cpp
        source/creatures/items/rock.cpp
        source/creatures/items/arrow.cpp
        source/creatures/items/bomb.hpp
        source/creatures/items/rope.hpp
        source/decorations/copyrights.cpp
        source/decorations/copyrights.h
        source/creatures/items/rope_element.hpp
        source/creatures/items/rope_element.cpp
        source/creatures/items/pistol.hpp
        source/creatures/items/pistol.cpp
        source/creatures/items/rope.cpp
        source/creatures/_base_creature.h
        source/creatures/_base_creature.cpp
        source/main.cpp
        source/tiles/level.cpp
        source/tiles/level.hpp
        source/tiles/level_generator.cpp
        source/tiles/level_generator.hpp
        source/tiles/map_tile.cpp
        source/tiles/map_tile.hpp
        source/game_loop.cpp
        source/game_loop.hpp
        Makefile
        README.md
        source/creatures/main_dude/main_dude.cpp
        source/creatures/main_dude/main_dude.hpp
        source/creatures/sprite_state.hpp
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
        source/creatures/sprite_info.cpp source/creatures/sprite_info.h source/camera/camera.cpp source/camera/camera.hpp source/memory/oam_manager.cpp source/memory/oam_manager.hpp source/input/input_handler.cpp source/input/input_handler.hpp source/creatures/spritesheet_type.hpp source/memory/oam_type.hpp source/creatures/items/rock.hpp source/creatures/items/jar.cpp source/creatures/items/jar.hpp source/creatures/enemies/snake.cpp source/creatures/enemies/snake.hpp source/interfaces/moniez.cpp source/interfaces/moniez.hpp source/tiles/map_tile_type.hpp source/creatures/animations/blood.cpp source/creatures/animations/blood.hpp source/creatures/animations/blood_element.cpp source/creatures/animations/blood_element.hpp source/decorations/spelunky_title.cpp source/decorations/spelunky_title.h source/tiles/tile_orientation.hpp source/creatures/menu_sign_type.hpp source/tiles/splash_screen_type.hpp source/rooms/left_right_rooms.hpp source/rooms/left_right_down_rooms.hpp source/rooms/left_right_up_rooms.hpp source/rooms/closed_rooms.hpp source/rooms/splash_screens.hpp source/rooms/entrance_rooms.hpp source/rooms/exit_rooms.hpp source/rooms/RoomType.hpp source/creatures/enemies/bat.cpp source/creatures/enemies/bat.hpp source/creatures/enemies/spider.cpp source/creatures/enemies/spider.hpp source/creatures/sprite_type.hpp source/globals_definitions.hpp source/sound/sound_utils.hpp source/console/console_utils.hpp source/time/time_utils.h source/time/time_utils.cpp source/memory/oam_utils.cpp source/memory/oam_utils.hpp source/creatures/main_dude/whip.cpp source/creatures/main_dude/whip.hpp source/creatures/traps/spikes.cpp source/creatures/traps/spikes.h source/creatures/collectibles/chest.cpp source/creatures/collectibles/chest.hpp source/creatures/collectibles/crate.hpp source/creatures/collectibles/crate.cpp source/creatures/collectibles/shotgun.hpp source/creatures/collectibles/shotgun.cpp source/creatures/collectibles/bullet.hpp source/creatures/collectibles/bullet.cpp source/creatures/collectibles/spring_shoes.hpp source/creatures/collectibles/spring_shoes.cpp source/creatures/collectibles/mitt.hpp source/creatures/collectibles/mitt.cpp source/creatures/collectibles/glove.hpp source/creatures/collectibles/glove.cpp source/creatures/collectibles/compass.hpp source/creatures/collectibles/compass.cpp source/decorations/got_collectible.hpp source/decorations/got_collectible.cpp source/camera/layer_level.hpp source/creatures/enemies/shopkeeper.hpp source/creatures/enemies/shopkeeper.cpp source/creatures/main_dude/dude_utils.cpp source/creatures/main_dude/game_state.hpp source/creatures/main_dude/game_state.cpp source/creatures/enemies/caveman.hpp source/creatures/enemies/caveman.cpp source/creatures/animations/fall_poof.hpp source/creatures/animations/fall_poof.cpp source/creatures/items/cape.hpp source/creatures/items/cape.cpp source/creatures/items/spike_shoes.hpp source/creatures/items/spike_shoes.cpp source/creatures/items/pick.hpp source/creatures/items/pick.cpp source/creatures/items/jetpack.hpp source/creatures/items/jetpack.cpp source/creatures/items/parachute.hpp source/creatures/items/parachute.cpp source/creatures/items/rope_pile.hpp source/creatures/items/rope_pile.cpp source/creatures/items/bomb_bag.hpp source/creatures/items/bomb_bag.cpp source/creatures/items/bomb_box.hpp source/creatures/items/bomb_box.cpp source/creatures/animations/flame.hpp source/creatures/animations/flame_element.cpp source/creatures/animations/flame.cpp source/creatures/animations/flame_element.hpp source/creatures/enemies/damsel.hpp source/creatures/enemies/damsel.cpp source/decorations/smooch.hpp source/decorations/smooch.cpp source/rooms/shops.hpp source/decorations/lamp.h source/decorations/lamp.cpp source/tiles/direction.hpp source/creatures/collectibles/collectibles_utils.hpp source/creatures/collectibles/collectibles_utils.cpp source/interfaces/shopping_object.h source/decorations/shopping_icon.h source/decorations/shopping_icon.cpp source/interfaces/shopping_object.cpp source/decorations/arrow_trap.h source/decorations/arrow_trap.cpp source/decorations/blast.hpp source/decorations/blast.cpp source/rooms/snake_well.hpp source/creatures/enemies/skeleton.cpp source/creatures/enemies/skeleton.hpp source/sound/sound_utils.cpp source/creatures/animations/bone.hpp source/creatures/animations/bone.cpp source/creatures/animations/skull.hpp source/creatures/animations/skull.cpp source/creatures/animations/fake_skeleton.hpp source/creatures/animations/fake_skeleton.cpp source/globals_definitions.cpp source/base_map.hpp source/creatures/sprite_utils.cpp source/creatures/sprite_utils.hpp source/tiles/level_generator.hpp source/tiles/level_generator.cpp source/creatures/simple_sprite_wrapper.h source/creatures/simple_sprite_wrapper.cpp source/tiles/populating_utils.hpp source/tiles/populating_utils.cpp source/creatures/collectibles/ruby_big.h source/creatures/collectibles/ruby_small.h source/creatures/collectibles/triple_goldbar.h source/creatures/collectibles/triple_goldbar.cpp source/creatures/collectibles/single_goldbar.hpp source/creatures/collectibles/single_goldbar.cpp source/creatures/collectibles/ruby_small.cpp source/creatures/collectibles/ruby_big.cpp source/collisions/collisions_utils.h source/collisions/collisions_utils.cpp source/decorations/_base_decoration.h source/decorations/_base_decoration.cpp source/decorations/rock_sign.h source/decorations/rock_sign.cpp source/decorations/arch_sign.h source/decorations/arch_sign.cpp source/interfaces/IRenderable.cpp source/interfaces/IRenderable.h source/interfaces/ICollidable.cpp source/interfaces/ICollidable.h source/interfaces/IBase.cpp source/interfaces/IBase.h source/treasures/BaseTreasure.cpp source/treasures/BaseTreasure.h)
