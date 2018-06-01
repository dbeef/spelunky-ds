//
// Created by xdbeef on 23.04.18.
//

#include <maxmod9.h>
#include "../items/jar.h"
#include "../moving_object.h"
#include "moniez.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_triple_goldbar.h"
#include "../../../build/gfx_rubies.h"
#include "../items/rock.h"
#include "../../../build/soundbank.h"
#include "../sprite_type.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"


void Moniez::draw() {

    if (ready_to_dispose)
        return;

    if (!collected && collectible_timer >= 500 &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(x, y, physical_width, physical_height, 8)) {
        global::hud->draw();

        if (spriteType == SpritesheetType::MONIEZ_RUBY) {
            mmEffect(SFX_XGEM);
        } else if (spriteType == SpritesheetType::MONIEZ_TRIPLE_GOLD_BARS) {
            mmEffect(SFX_XCOIN);
        }

        global::hud->collectedMoniez(value);
        collected = true;
        subSpriteInfo->entry->isHidden = true;
        mainSpriteInfo->entry->isHidden = true;

        if (spriteType == SpritesheetType::MONIEZ_RUBY) {
            if (ruby_type == 0) {
                global::collected_loot.push_back(SpriteType::S_MONIEZ_RUBY_BIG_RED);
            } else if (ruby_type == 1) {
                global::collected_loot.push_back(SpriteType::S_MONIEZ_RUBY_BIG_GREEN);
            } else if (ruby_type == 2) {
                global::collected_loot.push_back(SpriteType::S_MONIEZ_RUBY_BIG_BLUE);
            }
        } else {
            global::collected_loot.push_back(SpriteType::S_MONIEZ_TRIPLE_GOLD_BARS);
        }

        ready_to_dispose = true;
    }


    set_position();

    if (collectible_timer < 500)
        collectible_timer += *global::timer;

}


void Moniez::init() {
    ruby_type = rand() % 6;

    initSprite();

    activated_by_main_dude = true;
    collected = false;

}

void Moniez::updateSpeed() {

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

void Moniez::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    MapTile *tiles[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles, x_current_pos_in_tiles, y_current_pos_in_tiles,
                                    tiles);

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true,BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true, BOUNCING_FACTOR_X);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, BOUNCING_FACTOR_Y);

}

void Moniez::initSprite() {


    if (spriteType == SpritesheetType::MONIEZ_TRIPLE_GOLD_BARS) {

        physical_height = TRIPLE_GOLDBAR_PHYSICAL_HEIGHT;
        physical_width = TRIPLE_GOLDBAR_PHYSICAL_WIDTH;
        sprite_height = TRIPLE_GOLDBAR_SPRITE_HEIGHT;
        sprite_width = TRIPLE_GOLDBAR_SPRITE_WIDTH;


        subSpriteInfo = global::sub_oam_manager->initSprite(gfx_triple_goldbarPal, gfx_triple_goldbarPalLen,
                                                            nullptr, sprite_width * sprite_height, 16,
                                                            spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo = global::main_oam_manager->initSprite(gfx_triple_goldbarPal, gfx_triple_goldbarPalLen,
                                                              nullptr, sprite_width * sprite_height, 16,
                                                              spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);
        frameGfx = (u8 *) gfx_triple_goldbarTiles;

    } else if (spriteType == SpritesheetType::MONIEZ_RUBY) {

        if (ruby_type >= 3) {
            physical_height = RUBY_SMALL_PHYSICAL_HEIGHT;
            physical_width = RUBY_SMALL_PHYSICAL_WIDTH;
            sprite_height = RUBY_SMALL_SPRITE_HEIGHT;
            sprite_width = RUBY_SMALL_SPRITE_WIDTH;
        } else {
            physical_height = RUBY_BIG_PHYSICAL_HEIGHT;
            physical_width = RUBY_BIG_PHYSICAL_WIDTH;
            sprite_height = RUBY_BIG_SPRITE_HEIGHT;
            sprite_width = RUBY_BIG_SPRITE_WIDTH;
        }

        subSpriteInfo = global::sub_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen,
                                                            nullptr, sprite_width * sprite_height, 8,
                                                            spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo = global::main_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen,
                                                              nullptr, sprite_width * sprite_height, 8,
                                                              spriteType, true, false, LAYER_LEVEL::MIDDLE_TOP);

        frameGfx = (u8 *) gfx_rubiesTiles + 8 * 8 * (ruby_type) / 2;
    }

    subSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);
    mainSpriteInfo->updateFrame(frameGfx, sprite_width * sprite_height);

}

void Moniez::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    mainSpriteInfo->entry->x = main_x;
    mainSpriteInfo->entry->y = main_y;

    subSpriteInfo->entry->x = sub_x;
    subSpriteInfo->entry->y = sub_y;

    mainSpriteInfo->entry->vFlip = false;
    mainSpriteInfo->entry->hFlip = false;

    subSpriteInfo->entry->vFlip = false;
    subSpriteInfo->entry->hFlip = false;

}

Moniez::Moniez() {
    collectible_timer = 500;
}

