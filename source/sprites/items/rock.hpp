//
// Created by xdbeef on 23.04.18.
//

#ifndef SPELUNKYDS_ROCK_H
#define SPELUNKYDS_ROCK_H

#include <nds/arm9/sprite.h>
#include "../moving_object.h"

#define MAX_X_SPEED_ROCK 4
#define MAX_Y_SPEED_ROCK 4

#define ROCK_PHYSICAL_HEIGHT 7
#define ROCK_PHYSICAL_WIDTH 7

#define ROCK_SPRITE_HEIGHT 8
#define ROCK_SPRITE_WIDTH 8
#define ROCK_SPRITE_SIZE ROCK_SPRITE_WIDTH * ROCK_SPRITE_HEIGHT

//http://spelunky.wikia.com/wiki/Rock
class Rock : public MovingObject {

public:

    void introduce_yourself() override { printf("ROCK\n"); };

    Rock();
    
    void updateOther() override {};

    void initSprite() override;

    void deleteSprite() override;

    void init() override;

    void apply_dmg(int dmg_to_apply) override {};

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

    void set_position();

    void set_sprite_attributes();
};


#endif //SPELUNKYDS_ROCK_H
