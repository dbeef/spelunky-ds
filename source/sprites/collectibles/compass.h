//
// Created by xdbeef on 19.05.18.
//

#ifndef SPELUNKYDS_COMPASS_H
#define SPELUNKYDS_COMPASS_H

#include "../sprite_info.h"
#include "../sprite_state.hpp"
#include "../moving_object.h"
#include "../shopping_object.h"

#define COMPASS_PHYSICAL_HEIGHT 12
#define COMPASS_PHYSICAL_WIDTH 12

#define COMPASS_SPRITE_HEIGHT 16
#define COMPASS_SPRITE_WIDTH 16
#define COMPASS_SPRITE_SIZE COMPASS_SPRITE_WIDTH * COMPASS_SPRITE_HEIGHT

#define MAX_X_SPEED_COMPASS 4
#define MAX_Y_SPEED_COMPASS 4

class Compass : public MovingObject , public ShoppingObject {

public:

    void introduce_yourself() override { printf("COMPASS\n"); };

    Compass();

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

    u8 *frameGfx{};

    void set_position();

    void draw_arrow_to_exit();

    void equip();
};


#endif //SPELUNKYDS_COMPASS_H
