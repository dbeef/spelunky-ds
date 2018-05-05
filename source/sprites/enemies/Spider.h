//
// Created by xdbeef on 03.05.18.
//

#ifndef SPELUNKYDS_SPIDER_H
#define SPELUNKYDS_SPIDER_H

#include <nds/jtypes.h>
#include "../MovingObject.h"
#include "../SpriteState.h"
#include "../SpriteInfo.h"

class Spider : public MovingObject {

public:

    void initSprite() override;

    void updateOther() override {};

    void init() override;

    void draw() override;

    void kill() override ;

    void updateTimers() override {};

    void updatePosition() override;

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    double *timer = nullptr;
    double pos_inc_timer;

    SpriteInfo *mainSpriteInfo = nullptr;
    SpriteInfo *subSpriteInfo = nullptr;
    u8 *frameGfx;

    SpriteState spriteState;

    int animFrame;
    int animFrameTimer;

    double random_speed;

    bool hanging;
    bool hunting;
    bool jumping;

    void set_sprite_hanging();

    void set_sprite_flipping();

    void set_sprite_jumping();

    void set_sprite_falling();

    void set_position();
};



#endif //SPELUNKYDS_SPIDER_H
