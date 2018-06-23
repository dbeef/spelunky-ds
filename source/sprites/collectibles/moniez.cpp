//
// Created by xdbeef on 23.04.18.
//

#include <maxmod9.h>
#include "../items/jar.h"
#include "../moving_object.h"
#include "moniez.h"
#include "../../globals_declarations.h"
#include "../../../build/gfx_goldbars.h"
#include "../../../build/gfx_rubies.h"
#include "../items/rock.h"
#include "../../../build/soundbank.h"
#include "../sprite_type.h"
#include "../../collisions/collisions.h"
#include "../../tiles/map_utils.h"


void Moniez::draw() {

    if (ready_to_dispose) {
        mainSpriteInfo->entry->isHidden = true;
        subSpriteInfo->entry->isHidden = true;
        return;
    } else {
        mainSpriteInfo->entry->isHidden = false;
        subSpriteInfo->entry->isHidden = false;
    }


    if (!collected && collectible_timer >= 500 &&
        Collisions::checkCollisionWithMainDudeWidthBoundary(x, y, physical_width, physical_height, 8)) {
        global::hud->draw();

        if (spritesheet_type == SpritesheetType::MONIEZ_RUBY) {
            mmEffect(SFX_XGEM);
        } else if (spritesheet_type == SpritesheetType::MONIEZ_GOLDBARS) {
            mmEffect(SFX_XCOIN);
        }

        global::hud->collectedMoniez(value);
        collected = true;
        subSpriteInfo->entry->isHidden = true;
        mainSpriteInfo->entry->isHidden = true;

        global::collected_loot.push_back(sprite_type);

        ready_to_dispose = true;
    }


    set_position();

    if (collectible_timer < 500)
        collectible_timer += *global::timer;

}


void Moniez::init() {


    int type = rand() % 8;

    switch (type) {
        case 0:
            sprite_type = SpriteType::S_MONIEZ_RUBY_BIG_RED;
            break;
        case 1:
            sprite_type = SpriteType::S_MONIEZ_RUBY_BIG_GREEN;
            break;
        case 2:
            sprite_type = SpriteType::S_MONIEZ_RUBY_BIG_BLUE;
            break;
        case 3:
            sprite_type = SpriteType::S_MONIEZ_RUBY_SMALL_RED;
            break;
        case 4:
            sprite_type = SpriteType::S_MONIEZ_RUBY_SMALL_GREEN;
            break;
        case 5:
            sprite_type = SpriteType::S_MONIEZ_RUBY_SMALL_BLUE;
            break;
        case 6:
            sprite_type = SpriteType::S_MONIEZ_ONE_GOLDBAR;
            break;
        case 7:
            sprite_type = SpriteType::S_MONIEZ_TRIPLE_GOLDBARS;
            break;
        default:
            break;
    }

    initSprite();

    activated = true;
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

    bottomCollision = Collisions::checkBottomCollision(tiles, &x, &y, &ySpeed, physical_width, physical_height, true,
                                                       BOUNCING_FACTOR_Y);
    leftCollision = Collisions::checkLeftCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                   BOUNCING_FACTOR_X);
    rightCollision = Collisions::checkRightCollision(tiles, &x, &y, &xSpeed, physical_width, physical_height, true,
                                                     BOUNCING_FACTOR_X);
    upperCollision = Collisions::checkUpperCollision(tiles, &x, &y, &ySpeed, physical_width, true, BOUNCING_FACTOR_Y);

}

void Moniez::initSprite() {


    if (spritesheet_type == SpritesheetType::MONIEZ_GOLDBARS) {

        if (sprite_type == SpriteType::S_MONIEZ_TRIPLE_GOLDBARS) {
            physical_height = TRIPLE_GOLDBAR_PHYSICAL_HEIGHT;
            physical_width = TRIPLE_GOLDBAR_PHYSICAL_WIDTH;
            sprite_height = TRIPLE_GOLDBAR_SPRITE_HEIGHT;
            sprite_width = TRIPLE_GOLDBAR_SPRITE_WIDTH;
            value = 1000;
            frameGfx = (u8 *) gfx_goldbarsTiles;
        } else {
            physical_height = ONE_GOLDBAR_PHYSICAL_HEIGHT;
            physical_width = ONE_GOLDBAR_PHYSICAL_WIDTH;
            sprite_height = ONE_GOLDBAR_SPRITE_HEIGHT;
            sprite_width = ONE_GOLDBAR_SPRITE_WIDTH;
            value = 500;
            frameGfx = (u8 *) gfx_goldbarsTiles + ( sprite_width * sprite_height * 1/ 2);
        }

        subSpriteInfo = global::sub_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                            nullptr, sprite_width * sprite_height, 16,
                                                            spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo = global::main_oam_manager->initSprite(gfx_goldbarsPal, gfx_goldbarsPalLen,
                                                              nullptr, sprite_width * sprite_height, 16,
                                                              spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    } else if (spritesheet_type == SpritesheetType::MONIEZ_RUBY) {

        if (sprite_type == SpriteType::S_MONIEZ_RUBY_SMALL_BLUE || sprite_type == SpriteType::S_MONIEZ_RUBY_SMALL_GREEN
            || sprite_type == SpriteType::S_MONIEZ_RUBY_SMALL_RED) {
            physical_height = RUBY_SMALL_PHYSICAL_HEIGHT;
            physical_width = RUBY_SMALL_PHYSICAL_WIDTH;
            sprite_height = RUBY_SMALL_SPRITE_HEIGHT;
            sprite_width = RUBY_SMALL_SPRITE_WIDTH;
            value = 1000;
        } else {
            physical_height = RUBY_BIG_PHYSICAL_HEIGHT;
            physical_width = RUBY_BIG_PHYSICAL_WIDTH;
            sprite_height = RUBY_BIG_SPRITE_HEIGHT;
            sprite_width = RUBY_BIG_SPRITE_WIDTH;
            value = 1200;
        }

        subSpriteInfo = global::sub_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen,
                                                            nullptr, sprite_width * sprite_height, 8,
                                                            spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
        mainSpriteInfo = global::main_oam_manager->initSprite(gfx_rubiesPal, gfx_rubiesPalLen,
                                                              nullptr, sprite_width * sprite_height, 8,
                                                              spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

        int ruby_type;

        if (sprite_type == SpriteType::S_MONIEZ_RUBY_BIG_RED)
            ruby_type = 0;
        else if (sprite_type == SpriteType::S_MONIEZ_RUBY_BIG_GREEN)
            ruby_type = 1;
        else if (sprite_type == SpriteType::S_MONIEZ_RUBY_BIG_BLUE)
            ruby_type = 2;
        else if (sprite_type == SpriteType::S_MONIEZ_RUBY_SMALL_RED)
            ruby_type = 3;
        else if (sprite_type == SpriteType::S_MONIEZ_RUBY_SMALL_GREEN)
            ruby_type = 4;
        else if (sprite_type == SpriteType::S_MONIEZ_RUBY_SMALL_BLUE)
            ruby_type = 5;

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

