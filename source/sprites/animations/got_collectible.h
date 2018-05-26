//
// Created by xdbeef on 19.05.18.
//

#ifndef SPELUNKYDS_GOT_COLLECTIBLE_H
#define SPELUNKYDS_GOT_COLLECTIBLE_H


#include "../sprite_info.h"
#include "../sprite_state.h"
#include "../moving_object.h"

#define GOT_COLLECTIBLE_SPRITE_HEIGHT 32
#define GOT_COLLECTIBLE_SPRITE_WIDTH 32

class GotCollectible : public MovingObject {

public:

    GotCollectible();

    void updateOther() override {};

    void init() override;

    void draw() override;

    void initSprite() override;

    void kill() override {};

    void updateTimers() override {};

    void updateSpeed() override {};

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override {};

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    int animFrameTimer{};
    int animFrame{};
    int cycle_counter{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    u8 *frameGfx{};

    SpritesheetType spriteType{};

    void set_position();

    int collectible_type{};
};



#endif //SPELUNKYDS_GOT_COLLECTIBLE_H
