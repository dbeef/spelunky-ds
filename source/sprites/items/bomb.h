//
// Created by xdbeef on 04.04.18.
//

#ifndef SPELUNKYDS_BOMB_H
#define SPELUNKYDS_BOMB_H

#define BOMB_SIZE 8
#define ARMED_TIME_BLINK_SLOW 2000
#define ARMED_TIME_BLINK_FAST 3500

#define BOMB_PHYSICAL_HEIGHT 8
#define BOMB_PHYSICAL_WIDTH 8
#define BOMB_SPRITE_HEIGHT 64
#define BOMB_SPRITE_WIDTH 64

#define MAX_X_SPEED_BOMB 4
#define MAX_Y_SPEED_BOMB 4

#include <nds/arm9/sprite.h>
#include "../../camera/camera.h"
#include "../moving_object.h"
#include "../sprite_info.h"

class Bomb : public MovingObject {

public:

    void introduce_yourself() override { printf("BOMB\n"); };

    Bomb();

    void updateOther() override {};

    void init() override;

    void apply_dmg(int dmg_to_apply) override {};

    void draw() override;

    void initSprite() override;

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 * frameGfx{};

    bool armed{};

    int armedTimer{};
    int explosionTimer{};

    int explosionFrame{};

    void disarm();

    void arm();
};


#endif //SPELUNKYDS_BOMB_H
