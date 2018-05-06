//
// Created by xdbeef on 24.02.18.
//

#ifndef SPELUNKYDS_MAINDUDE_H
#define SPELUNKYDS_MAINDUDE_H

#define MAIN_DUDE_MAX_X_SPEED 2
#define MAIN_DUDE_MAX_X_SPEED_CRAWLING 1.5
#define MAIN_DUDE_MAX_Y_SPEED 3.3
#define MAIN_DUDE_JUMP_SPEED 2.55
#define MAIN_DUDE_HEIGHT 16
#define MAIN_DUDE_WIDTH 16

#define MIN_HANGING_TIME 100
#define STUN_TIME 2000
#define STUN_FALLING_TIME 1200
#define PUSHING_TIME 500
#define DAMAGE_PROTECTION_TIME 1500

#include <nds.h>
#include <vector>
#include "../sprite_state.h"
#include "../../tiles/map_tile.h"
#include "../../tiles/level_generator.h"
#include "../../camera/camera.h"
#include "../sprite_info.h"
#include "../items/bomb.h"
#include "../moving_object.h"
#include "../../input/input_handler.h"

class MainDude : public MovingObject {

public:

    void updateOther() override;

    void kill() override {};

    void draw() override;

    void initSprite() override;

    void updateTimers() override;

    void updatePosition() override;

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    void init() override;

    SpriteInfo *main_whip = nullptr;
    SpriteInfo *main_pre_whip = nullptr;
    SpriteInfo *sub_whip = nullptr;
    SpriteInfo *sub_pre_whip = nullptr;
    SpriteInfo *main_spelunker = nullptr;
    SpriteInfo *sub_spelunker = nullptr;

    int animation_frame_timer;
    int speed_inc_timer;
    int pos_inc_timer;
    //How much time was spent on hanging, since last flag hanging_on_tile_left/right occured.
    int hanging_timer;


    int stunned_timer;
    int jumping_timer;
    int pushing_timer;
    int whip_timer;
    int time_since_last_jump;
    int time_since_last_damage;

    bool hanging_on_tile_left;
    bool hanging_on_tile_right;
    bool stunned;
    bool crawling;
    bool pushing_left;
    bool pushing_right;
    bool whip;
    bool can_climb_rope;
    bool can_climb_ladder;
    bool climbing;
    bool on_top_of_climbing_space;
    bool exiting_level;
    bool holding_item;
    bool dead;

    SpriteState state;
    int animFrame;
    u8 *frameGfx = nullptr;

    void handle_key_input();

    void can_hang_on_tile(MapTile **neighboringTiles);


    void apply_crawling_sprite();

    void apply_hanging_on_tile_sprite();

    void apply_whip_sprite();

    void apply_pushing_sprite();

    void apply_stunned_sprite();

    void apply_climbing_sprite();

    void apply_dead_sprite();

    void apply_walking_sprite();

    void apply_falling_sprite();

    void apply_exiting_level_sprite();

    void apply_blinking_on_damage();

    void reset_values_checked_every_frame();
};


#endif //SPELUNKYDS_MAINDUDE_H
