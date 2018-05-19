//
// Created by xdbeef on 23.04.18.
//

#ifndef SPELUNKYDS_ROCK_H
#define SPELUNKYDS_ROCK_H

#define MAX_X_SPEED_ROCK 4
#define MAX_Y_SPEED_ROCK 4

#define ROCK_PHYSICAL_HEIGHT 7
#define ROCK_PHYSICAL_WIDTH 7
#define ROCK_SPRITE_HEIGHT 8
#define ROCK_SPRITE_WIDTH 8

#include <nds/arm9/sprite.h>

class Rock : public MovingObject {

public:

    Rock();
    
    void updateOther() override {};

    void initSprite() override;

    void init() override;

    void kill() override {};

    void draw() override;

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 * frameGfx{};

};


#endif //SPELUNKYDS_ROCK_H
