//
// Created by xdbeef on 17.05.18.
//

#include <maxmod9.h>
#include <cstdlib>
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "chest.h"
#include "../../collisions/collisions.h"
#include "../../globals_declarations.h"
#include "../../../build/soundbank.h"
#include "shotgun.h"
#include "bullet.h"
#include "../animations/got_collectible.h"
#include "../../tiles/map_utils.h"
#include "../sprite_utils.h"

#define SHOTGUN_POS_INC_DELTA 15
#define SHOTGUN_COOLDOWN 750

void Shotgun::draw() {

    if (ready_to_dispose) return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    update_anim_icon(x, y, physical_width);
    match_animation();

    check_if_can_be_pickuped();

    if (hold_by_main_dude) {

        if (shopping_transaction(this))
            equip();

        global::main_dude->carrying_shotgun = true;
        sprite_state = global::main_dude->sprite_state;
        sprite_utils::set_priority(OBJPRIORITY_0, mainSpriteInfo, subSpriteInfo);

        y = global::main_dude->y + 7;
        if (sprite_state == SpriteState::W_LEFT)
            x = global::main_dude->x - 4;
        else
            x = global::main_dude->x + 7;

    } else {
        sprite_utils::set_priority(OBJPRIORITY_1, mainSpriteInfo, subSpriteInfo);
        global::main_dude->carrying_shotgun = false;
    }

    set_position();
    handle_shooting();
    kill_mobs_if_thrown(1);
}


void Shotgun::init() {
    initSprite();
    init_anim_icon();
    update_anim_icon(x, y, physical_width);
    blast = new Blast();
    blast->init();
    global::sprites.push_back(blast);
}

void Shotgun::updateSpeed() {

    limit_speed(MAX_X_SPEED_SHOTGUN, MAX_Y_SPEED_SHOTGUN);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > SHOTGUN_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Shotgun::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {

    if (hold_by_anyone) return;

    MapTile *t[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);

}

void Shotgun::initSprite() {

    delete mainSpriteInfo;
    delete subSpriteInfo;

    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, SHOTGUN_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, SHOTGUN_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    match_animation();
    set_position();
}

void Shotgun::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

Shotgun::Shotgun() {
    cost = 13500;
    name = "SHOTGUN";
    physical_height = SHOTGUN_PHYSICAL_HEIGHT;
    physical_width = SHOTGUN_PHYSICAL_WIDTH;
    sprite_height = SHOTGUN_SPRITE_HEIGHT;
    sprite_width = SHOTGUN_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;
    sprite_type = SpriteType::S_SHOTGUN;
    //FIXME Set values of these fields in every MovingObject
}

void Shotgun::spawn_bullets() {

    for (int a = 0; a < 4; a++) {
        auto *b = new Bullet();
        b->x = x;
        b->y = y;

        if (sprite_state == SpriteState::W_LEFT) {
            b->xSpeed = -4.3 - ((rand() % 20) / 10.0);
            b->x -= 5;
        } else {
            b->xSpeed = 4.3 + ((rand() % 20) / 10.0);
            b->x += 5 + physical_width;
        }

        b->init();
        global::sprites_to_add.push_back(b);

        if (a == 0)
            b->ySpeed = 1;
        if (a == 1)
            b->ySpeed = -1;
        if (a == 3)
            b->ySpeed = 0;
        if (a == 4)
            b->ySpeed = 2;

    }
}

void Shotgun::equip() {
    auto *g = new GotCollectible();
    g->x = x - 12;
    g->y = y - 20;
    g->collectible_type = 0;
    g->init();
    global::sprites_to_add.push_back(g);

}

void Shotgun::match_animation() {

    if (sprite_state == SpriteState::W_LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, SHOTGUN_SPRITE_SIZE, 12);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, SHOTGUN_SPRITE_SIZE, 11);

    sprite_utils::update_frame(frameGfx, SHOTGUN_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

void Shotgun::handle_shooting() {
    if (activated && !firing && cooldown > SHOTGUN_COOLDOWN) {

        mmEffect(SFX_XSHOTGUN);
        firing = true;
        cooldown = 0;
        activated = false;
        spawn_bullets();

    } else
        activated = false;

    if (blast->animFrame >= 9) {
        firing = false;
        blast->animFrame = 0;
    }

    blast->firing = firing;

    if (!firing) {
        cooldown += *global::timer;
        if (sprite_state == SpriteState::W_LEFT)
            blast->x = x - 10;
        else
            blast->x = x + 10;
        blast->y = y;
        blast->sprite_state = sprite_state;
    }
}

