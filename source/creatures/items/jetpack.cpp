//
// Created by xdbeef on 02.06.18.
//

#include "../../../build/gfx_bat_snake_jetpack.h"
#include "../../globals_declarations.hpp"
#include "../../decorations/got_collectible.hpp"
#include "../../collisions/collisions.hpp"
#include "jetpack.hpp"
#include "../animations/fall_poof.hpp"
#include "../sprite_utils.hpp"

#define JETPACK_POS_INC_DELTA 15

//TODO Refactor

void Jetpack::draw() {

    if (global::main_dude->carrying_cape && collected) {
        global::main_dude->carrying_jetpack = false;
        ready_to_dispose = true;
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
    }

    if (ready_to_dispose)
        return;

    if (bought && !collected && check_if_can_be_equipped()) {
        equip();
    } else if (!bought && !hold_by_main_dude) {
        check_if_can_be_pickuped();
    }

    if (hold_by_main_dude) {
        set_pickuped_position(4, -4);
        if (shopping_transaction(this)) {
            collected = true;
            equip();
        }
    }

    if (collected) {

        if (global::main_dude->climbing || global::main_dude->exiting_level) {
            sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);
            set_pickuped_position_not_checking(-3, 2);
            sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
        } else if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
            sprite_utils::set_priority(OBJPRIORITY_1, mainSpriteInfo, subSpriteInfo);
            set_pickuped_position_not_checking(-6, 0);
            sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
        } else if (global::main_dude->sprite_state == SpriteState::W_RIGHT) {
            sprite_utils::set_priority(OBJPRIORITY_1, mainSpriteInfo, subSpriteInfo);
            set_pickuped_position_not_checking(-3, 0);
            sprite_utils::set_horizontal_flip(true, mainSpriteInfo, subSpriteInfo);
        }

        if (global::main_dude->using_jetpack) {
            poof_spawn_timer += *global::timer;

            if (global::main_dude->jetpack_fuel_counter <= 0) {
                global::main_dude->using_jetpack = false;
            }

            if (poof_spawn_timer > 130) {

                for (int a = 0; a < 2; a++) {
                    if (poofs[a] != nullptr) {
                        if (poofs[a]->ready_to_dispose) {

                            if (poofs[a]->mainSpriteInfo == nullptr || poofs[a]->subSpriteInfo == nullptr)
                                poofs[a]->initSprite();
                            else
                                sprite_utils::set_visibility(true, poofs[a]->mainSpriteInfo, poofs[a]->subSpriteInfo);

                            poofs[a]->animFrame = 0;
                            poofs[a]->animFrameTimer = 0;
                            poofs[a]->x = x + 4;
                            poofs[a]->y = y + 4;
                            poofs[a]->ready_to_dispose = false;

                            break;

                        }
                    } else {

                        auto *f = new FallPoof();
                        f->gravity = true;
                        f->x = x + 4;
                        f->y = y + 4;
                        f->init();
                        poofs[a] = f;
                        break;

                    }
                }

                poof_spawn_timer = 0;

            }

        }

        frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, JETPACK_SPRITE_SIZE, 8);
    } else {
        frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, JETPACK_SPRITE_SIZE, 7);
    }

    sprite_utils::update_frame(frameGfx, JETPACK_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_visibility(true, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);

    if (global::main_dude->bottomCollision || global::main_dude->hanging_on_tile_left ||
        global::main_dude->hanging_on_tile_right) {
        global::main_dude->using_jetpack = false;
        global::main_dude->jetpack_fuel_counter = 15;
    }

    set_position();

    for (int a = 0; a < 2; a++) {
        if (poofs[a] == nullptr)
            continue;
        poofs[a]->update();
        poofs[a]->draw();
    }
}


void Jetpack::init() {
    initSprite();
    init_anim_icon();

}

void Jetpack::updateSpeed() {

    limit_speed(MAX_X_SPEED_JETPACK, MAX_Y_SPEED_JETPACK);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > JETPACK_POS_INC_DELTA) && !collected;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Jetpack::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    if (collected) return;

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::current_level->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
}

void Jetpack::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                        nullptr, JETPACK_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_bat_snake_jetpackPal, gfx_bat_snake_jetpackPalLen,
                                                          nullptr, JETPACK_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    if (collected)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, JETPACK_SPRITE_SIZE, 8);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_bat_snake_jetpackTiles, JETPACK_SPRITE_SIZE, 7);

    sprite_utils::update_frame(frameGfx, JETPACK_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Jetpack::set_position() {

    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);

    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);

    update_anim_icon(x, y, physical_width);

}

Jetpack::Jetpack() {
    cost = 16000;
    name = "JETPACK";
    physical_height = JETPACK_PHYSICAL_HEIGHT;
    physical_width = JETPACK_PHYSICAL_WIDTH;
    sprite_height = JETPACK_SPRITE_HEIGHT;
    sprite_width = JETPACK_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::BAT_JETPACK;
}

void Jetpack::equip() {

    auto *g = new GotCollectible(x - 12, y - 20, GotCollectible::Type::ITEM);
    global::decorations_to_add.push_back(g);

    if (global::main_dude->carrying_cape) {
        global::main_dude->carrying_cape = false;
    }

    if (!global::main_dude->carrying_jetpack) {
        global::main_dude->carrying_jetpack = true;
        set_position();
        collected = true;
    } else {
        //we've collected an item that is already in inventory, dispose
        ready_to_dispose = true;
        sprite_utils::set_visibility(false, mainSpriteInfo, subSpriteInfo);
    }
}

void Jetpack::deleteSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    mainSpriteInfo = nullptr;
    subSpriteInfo = nullptr;

    for (int a = 0; a < 2; a++) {

        if (poofs[a] == nullptr)
            continue;

        poofs[a]->deleteSprite();
    }

}

Jetpack::~Jetpack() {

    deleteSprite();

    for (auto &poof : poofs)
        delete poof;

}
