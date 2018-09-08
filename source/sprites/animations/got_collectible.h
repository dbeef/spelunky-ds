//
// Created by xdbeef on 19.05.18.
//

#ifndef SPELUNKYDS_GOT_COLLECTIBLE_H
#define SPELUNKYDS_GOT_COLLECTIBLE_H


#include "../sprite_info.h"
#include "../sprite_state.hpp"
#include "../moving_object.h"

#define GOT_COLLECTIBLE_SPRITE_HEIGHT 32
#define GOT_COLLECTIBLE_SPRITE_WIDTH 32
#define GOT_COLLECTIBLE_SPRITE_SIZE GOT_COLLECTIBLE_SPRITE_WIDTH * GOT_COLLECTIBLE_SPRITE_HEIGHT

class GotCollectible : public MovingObject {

public:

    GotCollectible();

    void updateOther() override {};

    void init() override;

    void draw() override;

    void initSprite() override;

    void apply_dmg(int dmg_to_apply) override {};

    void updateTimers() override {};

    void updateSpeed() override {};

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override {};

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    double animFrameTimer{};
    int animFrame{};
    int cycle_counter{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    u8 *frameGfx{};

    void set_position();

    int collectible_type{};

    void match_animation();
};



#endif //SPELUNKYDS_GOT_COLLECTIBLE_H
