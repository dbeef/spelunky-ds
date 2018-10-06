//
// Created by xdbeef on 07.05.18.
//

#ifndef SPELUNKYDS_WHIP_H
#define SPELUNKYDS_WHIP_H

#define WHIP_PHYSICAL_WIDTH 16
#define WHIP_PHYSICAL_HEIGHT 16
#define WHIP_SPRITE_WIDTH 16
#define WHIP_SPRITE_HEIGHT 16
#define WHIP_SPRITE_SIZE WHIP_SPRITE_WIDTH * WHIP_SPRITE_HEIGHT 

#define WHIP_WIDTH 12

#include "../moving_object.h"

//http://spelunky.wikia.com/wiki/Whip
class Whip : public MovingObject {

public:

    void introduce_yourself() override { printf("WHIP\n"); };

    Whip();

    void updateOther() override {};

    void initSprite() override;

    void deleteSprite() override;

    void init() override;

    void apply_dmg(int dmg_to_apply) override {};

    void draw() override;

    void updateTimers() override {};

    void update_position() override ;

    void updateSpeed() override {};

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override {};

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    SpriteInfo *main_sprite_info {};
    SpriteInfo *sub_sprite_info {};

    u8 * frameGfx{};

    double whip_timer{};

    void assign_pre_whip_sprite();

    void assign_whip_sprite();

    void hide();

};




#endif //SPELUNKYDS_WHIP_H
