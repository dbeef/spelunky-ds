//
// Created by xdbeef on 23.06.18.
//

#ifndef SPELUNKYDS_BLAST_H
#define SPELUNKYDS_BLAST_H

#include "../sprite_info.h"
#include "../sprite_state.h"
#include "../moving_object.h"

#define BLAST_SPRITE_HEIGHT 16
#define BLAST_SPRITE_WIDTH 16

class Blast : public MovingObject {

public:

    Blast();

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

    int animFrameTimer{};
    int animFrame{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    u8 *frameGfx{};

    bool firing{};

    void set_position();
};




#endif //SPELUNKYDS_BLAST_H
