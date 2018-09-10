//
// Created by xdbeef on 02.06.18.
//

#ifndef SPELUNKYDS_FLAME_H
#define SPELUNKYDS_FLAME_H

#define MAX_X_SPEED_FLAME 1.5
#define MAX_Y_SPEED_FLAME 1.5

#define FLAME_PHYSICAL_HEIGHT 6
#define FLAME_PHYSICAL_WIDTH 6

#define FLAME_SPRITE_HEIGHT 16
#define FLAME_SPRITE_WIDTH 16
#define FLAME_SPRITE_SIZE FLAME_SPRITE_WIDTH * FLAME_SPRITE_HEIGHT

#define FLAME_CHANGE_POS_DELTA 16
#define FLAME_ANIM_FRAME_DELTA 90

#include <vector>
#include "../moving_object.h"
#include "../sprite_info.h"
#include "flame_element.h"

//almost same as blood animation

class Flame : public MovingObject {

public:

    void introduce_yourself() override { printf("FLAME\n"); };

    Flame();

    void updateOther() override {};

    void init() override;

    void apply_dmg(int dmg_to_apply) override {};

    void initSprite() override;

    void draw() override;

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};


    double *timer {};
    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};

    u8 *frameGfx{};

    bool finished{};
    int currentFrame{};
    double animFrameTimer{};

    std::vector<FlameElement *> flame_trail;

    double living_timer{};
    double time_since_last_spawn{};

    int change_pos_delta_offset{};

    void spawn_flame();

    void set_position();

    void match_animation();
};



#endif //SPELUNKYDS_FLAME_H
