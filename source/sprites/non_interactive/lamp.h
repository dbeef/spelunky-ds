//
// Created by xdbeef on 14.06.18.
//

#ifndef SPELUNKYDS_LAMP_H
#define SPELUNKYDS_LAMP_H

#define LAMP_SPRITE_HEIGHT 16
#define LAMP_SPRITE_WIDTH 16

#define LAMP_PHYSICAL_HEIGHT 16
#define LAMP_PHYSICAL_WIDTH 16

#include "../moving_object.h"

class Lamp : public MovingObject {

public:

    Lamp();

    void initSprite() override;

    void updateOther() override {};

    void apply_dmg(int dmg_to_apply) override {};

    void init() override;

    void draw() override;

    void updateTimers() override {};

    void update_position() override {};

    void updateSpeed() override {};

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override {};

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    int anim_frame{};
    int anim_frame_timer{};

};



#endif //SPELUNKYDS_LAMP_H
