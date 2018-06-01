//
// Created by xdbeef on 23.04.18.
//

#ifndef SPELUNKYDS_JAR_H
#define SPELUNKYDS_JAR_H

#define JAR_PHYSICAL_HEIGHT 9
#define JAR_PHYSICAL_WIDTH 10
#define JAR_SPRITE_HEIGHT 16
#define JAR_SPRITE_WIDTH 16

#define MAX_X_SPEED_JAR 4
#define MAX_Y_SPEED_JAR 4

#include "../moving_object.h"

class Jar : public MovingObject {

public:

    Jar();

    void updateOther() override {};

    void init() override;

    void draw() override;

    void apply_dmg(int dmg_to_apply) override;

    void updateTimers() override {};

    void updateSpeed() override;

    void initSprite() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};


    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 * frameGfx{};

    int frame{};
    int frameTimer{};

};



#endif //SPELUNKYDS_JAR_H
