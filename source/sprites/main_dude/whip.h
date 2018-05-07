//
// Created by xdbeef on 07.05.18.
//

#ifndef SPELUNKYDS_WHIP_H
#define SPELUNKYDS_WHIP_H

#define WHIP_SPRITE_WIDTH 16
#define WHIP_SPRITE_HEIGHT 16

#include "../moving_object.h"

class Whip : public MovingObject {

public:

    Whip();

    void updateOther() override {};

    void initSprite() override;

    void init() override;

    void kill() override {};

    void draw() override;

    void updateTimers() override {};

    void updatePosition() override ;

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    SpriteInfo *main_sprite_info = nullptr;
    SpriteInfo *sub_sprite_info = nullptr;

    int whip_timer;

    void assign_pre_whip_sprite();

    void assign_whip_sprite();

    void hide();

    void show();
};




#endif //SPELUNKYDS_WHIP_H
