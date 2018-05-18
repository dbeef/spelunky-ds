//
// Created by xdbeef on 16.05.18.
//

#ifndef SPELUNKYDS_CHEST_H
#define SPELUNKYDS_CHEST_H

#include "../moving_object.h"

#define CHEST_PHYSICAL_HEIGHT 8
#define CHEST_PHYSICAL_WIDTH 12
#define CHEST_SPRITE_HEIGHT 16
#define CHEST_SPRITE_WIDTH 16

#define MAX_X_SPEED_CHEST 4
#define MAX_Y_SPEED_CHEST 4


class Chest: public MovingObject {

public:

    Chest();

    void updateOther() override {};

    void init() override;

    void draw() override;

    void initSprite() override;

    void kill() override {};

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};
    
    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 * frameGfx{};

    SpritesheetType  spriteType{};
    void set_position();
};




#endif //SPELUNKYDS_CHEST_H
