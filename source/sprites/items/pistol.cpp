//
// Created by xdbeef on 18.05.18.
//


#include <maxmod9.h>
#include "pistol.h"
#include "../../globals_declarations.h"
#include "../../collisions/collisions.h"
#include "../../../build/gfx_spike_collectibles_flame.h"
#include "../../../build/soundbank.h"
#include "../animations/got_collectible.h"
#include "../../tiles/map_utils.h"
#include "../sprite_utils.h"
#include "../animations/blast.h"
#include "../collectibles/bullet.h"

#define PISTOL_POS_INC_DELTA 15
#define PISTOL_COOLDOWN 750

void Pistol::draw() {

    if (ready_to_dispose) return;

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    update_anim_icon(x, y, physical_width);
    match_animation();

    check_if_can_be_pickuped();
    if (hold_by_main_dude) {

        if (shopping_transaction(this))
            equip();

        global::main_dude->carrying_pistol = true;
        sprite_state = global::main_dude->sprite_state;

        y = global::main_dude->y + 7;
        if (global::main_dude->sprite_state == SpriteState::W_LEFT)
            x = global::main_dude->x - 4;
        else
            x = global::main_dude->x + 10;

    } else
        global::main_dude->carrying_pistol = false;

    set_position(); //set position must be called here, after position is offsetted if pistol carried
    handle_shooting();
    kill_mobs_if_thrown(1);
}


void Pistol::init() {
    initSprite();
    init_anim_icon();
    update_anim_icon(x, y, physical_width);
    blast = new Blast();
    blast->init();
    global::sprites_to_add.push_back(blast);
}

void Pistol::updateSpeed() {

    limit_speed(MAX_X_SPEED_PISTOL, MAX_Y_SPEED_PISTOL);

    pos_inc_timer += *global::timer;

    bool change_pos = (pos_inc_timer > PISTOL_POS_INC_DELTA) && !hold_by_main_dude;

    if (change_pos) {
        update_position();
        apply_friction(0.055);
        apply_gravity(GRAVITY_DELTA_SPEED);
        pos_inc_timer = 0;
    }

}

void Pistol::updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) {
    MapTile *t[9];
    Collisions::getNeighboringTiles(global::level_generator->map_tiles,
                                    x_current_pos_in_tiles, y_current_pos_in_tiles, t);

    upperCollision = Collisions::checkUpperCollision(t, &x, &y, &ySpeed, physical_width, true, 0.35);
    bottomCollision = Collisions::checkBottomCollision(t, &x, &y, &ySpeed, physical_width, physical_height, true, 0.35);
    leftCollision = Collisions::checkLeftCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
    rightCollision = Collisions::checkRightCollision(t, &x, &y, &xSpeed, physical_width, physical_height, true, 0.15);
}

void Pistol::initSprite() {

    mainSpriteInfo = global::main_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                          gfx_spike_collectibles_flamePalLen,
                                                          nullptr, PISTOL_SPRITE_SIZE, sprite_width,
                                                          spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);
    subSpriteInfo = global::sub_oam_manager->initSprite(gfx_spike_collectibles_flamePal,
                                                        gfx_spike_collectibles_flamePalLen,
                                                        nullptr, PISTOL_SPRITE_SIZE, sprite_width,
                                                        spritesheet_type, true, false, LAYER_LEVEL::MIDDLE_TOP);

    sprite_utils::set_vertical_flip(false, mainSpriteInfo, subSpriteInfo);
    sprite_utils::set_horizontal_flip(false, mainSpriteInfo, subSpriteInfo);
    match_animation();
    set_position();
}

void Pistol::set_position() {
    int main_x, main_y, sub_x, sub_y;
    get_x_y_viewported(&main_x, &main_y, &sub_x, &sub_y);
    sprite_utils::set_entry_xy(mainSpriteInfo, main_x, main_y);
    sprite_utils::set_entry_xy(subSpriteInfo, sub_x, sub_y);
}

Pistol::Pistol() {
    cost = 10 * 1000;
    name = "PISTOL";
    physical_height = PISTOL_PHYSICAL_HEIGHT;
    physical_width = PISTOL_PHYSICAL_WIDTH;
    sprite_height = PISTOL_SPRITE_HEIGHT;
    sprite_width = PISTOL_SPRITE_WIDTH;
    spritesheet_type = SpritesheetType::SPIKES_COLLECTIBLES;
}

void Pistol::equip() {
    auto *g = new GotCollectible();
    g->x = x - 12;
    g->y = y - 20;
    g->collectible_type = 0;
    g->init();
    global::sprites_to_add.push_back(g);
}

void Pistol::spawn_bullet() {

    auto *b = new Bullet();
    b->x = x;
    b->y = y;

    if (global::main_dude->sprite_state == SpriteState::W_LEFT) {
        b->xSpeed = -5.0;
        b->x -= 2;
    } else if (global::main_dude->sprite_state == SpriteState::W_RIGHT) {
        b->xSpeed = 5.0;
        b->x += 2;
    }

    b->init();
    global::sprites_to_add.push_back(b);
    b->ySpeed = 0;
}

//TODO Share this function between pistol / shotgun, similiar behaviour besides cooldown time / spawn bullet callback
void Pistol::handle_shooting() {

    if (activated && !firing && cooldown > PISTOL_COOLDOWN) {

        mmEffect(SFX_XSHOTGUN);
        firing = true;
        cooldown = 0;
        activated = false;
        spawn_bullet();

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

void Pistol::match_animation() {

    if (sprite_state == SpriteState::W_LEFT)
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, PISTOL_SPRITE_SIZE, 32);
    else
        frameGfx = sprite_utils::get_frame((u8 *) gfx_spike_collectibles_flameTiles, PISTOL_SPRITE_SIZE, 33);

    sprite_utils::update_frame(frameGfx, PISTOL_SPRITE_SIZE, mainSpriteInfo, subSpriteInfo);
}

