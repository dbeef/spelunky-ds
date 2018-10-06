//
// Created by xdbeef on 16.05.18.
//

#ifndef SPELUNKYDS_CRATE_H
#define SPELUNKYDS_CRATE_H


#include "../moving_object.h"
#include "../shopping_object.h"

#define CRATE_PHYSICAL_HEIGHT 10
#define CRATE_PHYSICAL_WIDTH 12

#define CRATE_SPRITE_HEIGHT 16
#define CRATE_SPRITE_WIDTH 16
#define CRATE_SPRITE_SIZE CRATE_SPRITE_WIDTH * CRATE_SPRITE_HEIGHT

#define MAX_X_SPEED_CRATE 4
#define MAX_Y_SPEED_CRATE 4

//http://spelunky.wikia.com/wiki/Crate
class Crate: public MovingObject {

public:

    void introduce_yourself() override { printf("CRATE\n"); };

    Crate();

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

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 * frameGfx{};

    bool dropped_loot{};
    int animFrame{};
    double animFrameTimer{};

    void set_position();

    void drop_loot();

    void play_collectible_animation();
};





#endif //SPELUNKYDS_CRATE_H
