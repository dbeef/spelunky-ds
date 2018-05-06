//
// Created by xdbeef on 28.04.18.
//

#ifndef SPELUNKYDS_BLOOD_H
#define SPELUNKYDS_BLOOD_H

#define BLOOD_SIZE 8
#define MAX_X_SPEED_BLOOD 2
#define MAX_Y_SPEED_BLOOD 2


#include <vector>
#include <nds/jtypes.h>
#include "../moving_object.h"
#include "blood_element.h"
#include "../sprite_info.h"


//blood trail animation:
//spawn 3-4 sprites with sBlood animation (3 frames)
//every of them spawn every .5 second animation sBloodTrail, every one of them should be one frame in forward, comprared to the latter
//if spawned 6 of them, then the sBlood animation sprites should finish with sBloodTrail animation

class Blood : public MovingObject {

public:

    void updateOther() override {};

    void init() override;

    void kill() override {};

    void initSprite() override;

    void draw() override;

    void updateTimers() override {};

    void updatePosition() override ;

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override;


    double *timer = nullptr;
    double pos_inc_timer;

    SpriteInfo *mainSpriteInfo = nullptr;
    SpriteInfo *subSpriteInfo = nullptr;

    u8 *frameGfx;

    bool finished;
    int currentFrame;
    int frameTimer;


    std::vector<BloodElement *> bloodTrail;

};



#endif //SPELUNKYDS_BLOOD_H
