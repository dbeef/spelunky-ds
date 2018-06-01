//
// Created by xdbeef on 28.04.18.
//

#ifndef SPELUNKYDS_BLOOD_H
#define SPELUNKYDS_BLOOD_H

#define MAX_X_SPEED_BLOOD 1.5
#define MAX_Y_SPEED_BLOOD 1.5

#define BLOOD_PHYSICAL_HEIGHT 4
#define BLOOD_PHYSICAL_WIDTH 4
#define BLOOD_SPRITE_HEIGHT 8
#define BLOOD_SPRITE_WIDTH 8

#define BLOOD_CHANGE_POS_DELTA 17
#define BLOOD_ANIM_FRAME_DELTA 90

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

    Blood();

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
    int animFrameTimer{};

    std::vector<BloodElement *> bloodTrail;

    int living_timer{};
    int time_since_last_spawn{};

    void spawn_blood();
};



#endif //SPELUNKYDS_BLOOD_H
