//
// Created by xdbeef on 19.05.18.
//

#ifndef SPELUNKYDS_GLOVE_H
#define SPELUNKYDS_GLOVE_H


#include "../sprite_info.h"
#include "../sprite_state.hpp"
#include "../moving_object.h"
#include "../shopping_object.h"

#define GLOVE_PHYSICAL_HEIGHT 15
#define GLOVE_PHYSICAL_WIDTH 16

#define GLOVE_SPRITE_HEIGHT 16
#define GLOVE_SPRITE_WIDTH 16
#define GLOVE_SPRITE_SIZE GLOVE_SPRITE_WIDTH * GLOVE_SPRITE_HEIGHT

#define MAX_X_SPEED_GLOVE 4
#define MAX_Y_SPEED_GLOVE 4

//http://spelunky.wikia.com/wiki/Climbing_Gloves
class Glove : public MovingObject , public ShoppingObject {

public:

    void introduce_yourself() override { printf("GLOVE\n"); };

    Glove();

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



#endif //SPELUNKYDS_GLOVE_H
