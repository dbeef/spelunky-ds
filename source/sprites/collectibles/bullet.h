//
// Created by xdbeef on 17.05.18.
//

#ifndef SPELUNKYDS_BULLET_H
#define SPELUNKYDS_BULLET_H

#include "../moving_object.h"
#include "../sprite_state.h"

#define BULLET_PHYSICAL_HEIGHT 4
#define BULLET_PHYSICAL_WIDTH 4
#define BULLET_SPRITE_HEIGHT 16
#define BULLET_SPRITE_WIDTH 16

#define MAX_X_SPEED_BULLET 8
#define MAX_Y_SPEED_BULLET 8

class Bullet : public MovingObject {

public:

    Bullet();

    void updateOther() override {};

    void init() override;

    void draw() override;

    void initSprite() override;

    void apply_dmg(int dmg_to_apply) override {};

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    void set_position();

    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    u8 *frameGfx{};

    int animFrame{};
    int animFrameTimer{};

    SpritesheetType spriteType{};

    void apply_bullet_dispose_animation();
};


#endif //SPELUNKYDS_BULLET_H
