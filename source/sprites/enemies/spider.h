//
// Created by xdbeef on 03.05.18.
//

#ifndef SPELUNKYDS_SPIDER_H
#define SPELUNKYDS_SPIDER_H

#define MAX_X_SPEED_SPIDER 4
#define MAX_Y_SPEED_SPIDER 4

#define SPIDER_PHYSICAL_HEIGHT 8
#define SPIDER_PHYSICAL_WIDTH 16
#define SPIDER_SPRITE_HEIGHT 16
#define SPIDER_SPRITE_WIDTH 16

#include <nds/jtypes.h>
#include "../moving_object.h"
#include "../sprite_state.h"
#include "../sprite_info.h"

class Spider : public MovingObject {

public:

    Spider();

    void initSprite() override;

    void updateOther() override {};

    void init() override;

    void draw() override;

    void kill() override;

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 *frameGfx{};

    SpriteState spriteState{};

    int animFrame{};
    int animFrameTimer{};

    double random_speed{};

    bool hanging{};
    bool hunting{};
    bool jumping{};

    void set_sprite_hanging();

    void set_sprite_flipping();

    void set_sprite_jumping();

    void set_sprite_falling();

    void set_position();

    void jump_to_main_dude();
};


#endif //SPELUNKYDS_SPIDER_H
