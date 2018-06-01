//
// Created by xdbeef on 25.05.18.
//

#ifndef SPELUNKYDS_SHOPKEEPER_H
#define SPELUNKYDS_SHOPKEEPER_H

#define MAX_X_SPEED_SHOPKEEPER 4
#define MAX_Y_SPEED_SHOPKEEPER 4

#define SHOPKEEPER_PHYSICAL_HEIGHT 16
#define SHOPKEEPER_PHYSICAL_WIDTH 16
#define SHOPKEEPER_SPRITE_HEIGHT 16
#define SHOPKEEPER_SPRITE_WIDTH 16

#include <nds/jtypes.h>
#include "../moving_object.h"
#include "../sprite_state.h"
#include "../sprite_info.h"

class Shopkeeper : public MovingObject {

public:

    Shopkeeper();

    void updateOther() override {};

    void init() override;

    void draw() override;

    void apply_dmg(int dmg_to_apply) override ;

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    void initSprite() override;

    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 *frameGfx{};

    SpriteState spriteState{};

    int animFrame{};
    int animFrameTimer{};
    
    void set_position();
};




#endif //SPELUNKYDS_SHOPKEEPER_H
