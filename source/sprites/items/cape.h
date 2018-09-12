//
// Created by xdbeef on 02.06.18.
//

#ifndef SPELUNKYDS_CAPE_H
#define SPELUNKYDS_CAPE_H


#include "../sprite_info.h"
#include "../sprite_state.hpp"
#include "../moving_object.h"
#include "../shopping_object.h"

#define CAPE_PHYSICAL_HEIGHT 15
#define CAPE_PHYSICAL_WIDTH 14

#define CAPE_SPRITE_HEIGHT 16
#define CAPE_SPRITE_WIDTH 16
#define CAPE_SPRITE_SIZE CAPE_SPRITE_WIDTH * CAPE_SPRITE_HEIGHT

#define MAX_X_SPEED_CAPE 4
#define MAX_Y_SPEED_CAPE 4

class Cape : public MovingObject , public ShoppingObject {

public:

    void introduce_yourself() override { printf("CAPE\n"); };

    Cape();

    void updateOther() override {};

    void init() override;

    void draw() override;

    void initSprite() override;

    void apply_dmg(int dmg_to_apply) override {};

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    double pos_inc_timer{};

    bool collected{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    int anim_frame_timer{};
    int anim_frame;

    u8 *frameGfx{};

    void set_position();

    void match_animation();

    void equip();
};




#endif //SPELUNKYDS_CAPE_H
