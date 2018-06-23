//
// Created by xdbeef on 23.04.18.
//

#ifndef SPELUNKYDS_ARROW_H
#define SPELUNKYDS_ARROW_H

#define MAX_X_SPEED_ARROW 4.5
#define MAX_Y_SPEED_ARROW 4

#define ARROW_PHYSICAL_HEIGHT 8
#define ARROW_PHYSICAL_WIDTH 8
#define ARROW_SPRITE_HEIGHT 8
#define ARROW_SPRITE_WIDTH 8

#include <nds/arm9/sprite.h>
#include "../moving_object.h"

class Arrow : public MovingObject {

public:

    Arrow();
    
    void updateOther() override {};

    void initSprite() override;

    void init() override;

    void apply_dmg(int dmg_to_apply) override {};

    void draw() override;

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 * frameGfx{};

    double angle{};

    void update_frame(int frame_num);
};


#endif //SPELUNKYDS_ARROW_H
