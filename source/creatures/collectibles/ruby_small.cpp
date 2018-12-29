//
// Created by xdbeef on 16.09.18.
//

#include "ruby_small.h"
#include "triple_goldbar.h"
#include <maxmod9.h>
#include <cstdlib>
#include "../items/jar.hpp"
#include "../_base_creature.h"
#include "moniez.hpp"
#include "../../globals_declarations.hpp"
#include "../../../build/gfx_goldbars.h"
#include "../../../build/gfx_rubies.h"
#include "../items/rock.hpp"
#include "../../../build/soundbank.h"
#include "../sprite_type.hpp"
#include "../../collisions/collisions.hpp"
#include "../../tiles/level_rendering_utils.hpp"
#include "../sprite_utils.hpp"

void RubySmall::draw() {

    if (ready_to_dispose) return;

    set_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    if (!collected && collectible_timer >= 500 &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(x, y, physical_width, physical_height, 8)) {

        if (spritesheet_type == SpritesheetType::MONIEZ_RUBY)
            mmEffect(SFX_XGEM);
        else if (spritesheet_type == SpritesheetType::MONIEZ_GOLDBARS)
            mmEffect(SFX_XCOIN);

        global::hud->add_moniez_on_collected_loot(value);
        collected = true;
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
        global::collected_loot.push_back(sprite_type);
        ready_to_dispose = true;
    }

    if (collectible_timer < 500)
        collectible_timer += *global::timer;

}


void RubySmall::init() {

    physical_height = RUBY_SMALL_PHYSICAL_HEIGHT;
    physical_width = RUBY_SMALL_PHYSICAL_WIDTH;
    sprite_height = RUBY_SMALL_SPRITE_HEIGHT;
    sprite_width = RUBY_SMALL_SPRITE_WIDTH;
    value = VALUE_RUBY_SMALL;
    spritesheet_type = SpritesheetType::MONIEZ_RUBY;

    //randomly select ruby color
    int type = rand() % 3;

    switch (type) {
        case 0:
            sprite_type = SpriteType::S_MONIEZ_RUBY_SMALL_RED;
            break;
        case 1:
            sprite_type = SpriteType::S_MONIEZ_RUBY_SMALL_GREEN;
            break;
        case 2:
            sprite_type = SpriteType::S_MONIEZ_RUBY_SMALL_BLUE;
            break;
        default:
            break;
    }

    initSprite();

    activated = true;
    collected = false;
}

void RubySmall::updateSpeed() {

    limit_speed(MAX_X_SPEED_MONIEZ, MAX_Y_SPEED_MONIEZ);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > 15) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;

    }

}

void RubySmall::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
}

void RubySmall::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen,
                                                        nullptr, RUBY_SMALL_SPRITE_SIZE, 8,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen, nullptr,
                                                          RUBY_SMALL_SPRITE_SIZE, 8, spritesheet_type, true,
                                                          false, LAYER_LEVEL::MIDDLE_TOP);

    int ruby_type{};

    if (sprite_type == SpriteType::S_MONIEZ_RUBY_SMALL_RED)
        ruby_type = 3;
    else if (sprite_type == SpriteType::S_MONIEZ_RUBY_SMALL_GREEN)
        ruby_type = 4;
    else if (sprite_type == SpriteType::S_MONIEZ_RUBY_SMALL_BLUE)
        ruby_type = 5;

    frameGfx = (u8 *) gfx_rubiesTiles + 8 * 8 * (ruby_type) / 2;

    sprite_utils::update_frame(frameGfx, RUBY_SMALL_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);

    set_position();
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);

}

void RubySmall::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

RubySmall::RubySmall() {
    collectible_timer = 500;
}

RubySmall::RubySmall(int x, int y, SpriteType sprite_type) : Moniez() {
    this->x = x;
    this->y = y;
    this->sprite_type = sprite_type;
}

void RubySmall::deleteSprite() {
    delete mainSpriteInfo;
    delete subSpriteInfo;
    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;
}
