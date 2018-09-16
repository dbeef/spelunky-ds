//
// Created by xdbeef on 03.05.18.
// http://spelunky.wikia.com/wiki/Spider
//

#ifndef SPELUNKYDS_SPIDER_H
#define SPELUNKYDS_SPIDER_H

#define MAX_X_SPEED_SPIDER 4
#define MAX_Y_SPEED_SPIDER 4

#define SPIDER_PHYSICAL_HEIGHT 8
#define SPIDER_PHYSICAL_WIDTH 16

#define SPIDER_SPRITE_HEIGHT 16
#define SPIDER_SPRITE_WIDTH 16
#define SPIDER_SPRITE_SIZE SPIDER_SPRITE_WIDTH * SPIDER_SPRITE_HEIGHT

#include "../moving_object.h"
#include "../sprite_state.hpp"
#include "../sprite_info.h"

//http://spelunky.wikia.com/wiki/Spider
class Spider : public MovingObject {

public:

    void introduce_yourself() override { printf("SPIDER\n"); };

    Spider();

    Spider(int x, int y);

    void initSprite() override;

    void updateOther() override {};

    void init() override;

    void draw() override;

    void apply_dmg(int dmg_to_apply) override;

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 *frameGfx{};

    int animFrame{};
    double animFrameTimer{};
    double time_since_last_jump{};
    double jump_delay{};
    double random_speed{};
    double time_since_last_big_jump{};

    bool previously_collided{};
    SpriteState previous_collision_side{};

    bool hanging{};
    bool hunting{};
    bool jumping{};

    void set_sprite_hanging();

    void set_sprite_flipping();

    void set_sprite_jumping();

    void set_sprite_falling();

    void set_position();

    void jump_to_main_dude();

    void match_animation();
};


#endif //SPELUNKYDS_SPIDER_H
