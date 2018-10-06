//
// Created by xdbeef on 29.04.18.
//

#ifndef SPELUNKYDS_STATICSPRITE_H
#define SPELUNKYDS_STATICSPRITE_H

#define SPELUNKY_TITLE_SPRITE_HEIGHT 64
#define SPELUNKY_TITLE_SPRITE_WIDTH 64
#define SPELUNKY_TITLE_SPRITE_SIZE SPELUNKY_TITLE_SPRITE_WIDTH * SPELUNKY_TITLE_SPRITE_HEIGHT

#define SPELUNKY_TITLE_PHYSICAL_HEIGHT 64
#define SPELUNKY_TITLE_PHYSICAL_WIDTH 64

#include "../moving_object.h"
#include "../sprite_info.h"

class SpelunkyTitle : public MovingObject {

public:

    void introduce_yourself() override { printf("SPELUNKY_TITLE\n"); };

    SpelunkyTitle();

    void updateOther() override {};

    void apply_dmg(int dmg_to_apply) override {};

    void initSprite() override;

    void deleteSprite() override;

    void init() override;

    void draw() override;

    void updateTimers() override {};

    void update_position() override {};

    void updateSpeed() override {};

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override {};

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    SpriteInfo *mainSpriteInfo_1 {};
    SpriteInfo *mainSpriteInfo_2 {};
    SpriteInfo *mainSpriteInfo_3 {};

    SpriteInfo *subSpriteInfo_1 {};
    SpriteInfo *subSpriteInfo_2 {};
    SpriteInfo *subSpriteInfo_3 {};

    OamType oamType{};
};




#endif //SPELUNKYDS_STATICSPRITE_H
