//
// Created by xdbeef on 19.05.18.
//

#ifndef SPELUNKYDS_MITT_H
#define SPELUNKYDS_MITT_H


#include "../sprite_info.h"
#include "../sprite_state.hpp"
#include "../moving_object.h"
#include "../shopping_object.h"

#define MITT_PHYSICAL_HEIGHT 14
#define MITT_PHYSICAL_WIDTH 14

#define MITT_SPRITE_HEIGHT 16
#define MITT_SPRITE_WIDTH 16
#define MITT_SPRITE_SIZE MITT_SPRITE_WIDTH * MITT_SPRITE_HEIGHT

#define MAX_X_SPEED_MITT 4
#define MAX_Y_SPEED_MITT 4

//http://spelunky.wikia.com/wiki/Pitcher%27s_Mitt
class Mitt : public MovingObject, public ShoppingObject  {

public:

    void introduce_yourself() override { printf("MITT\n"); };

    Mitt();

    void updateOther() override {};

    void init() override;

    void draw() override;

    void initSprite() override;

    void deleteSprite() override;

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

    void equip();

    void match_animation();
};


#endif //SPELUNKYDS_MITT_H
