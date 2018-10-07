//
// Created by xdbeef on 02.06.18.
//

#ifndef SPELUNKYDS_JETPACK_H
#define SPELUNKYDS_JETPACK_H


#include "../sprite_info.h"
#include "../sprite_state.hpp"
#include "../moving_object.h"
#include "../shopping_object.h"
#include "../animations/fall_poof.hpp"

#define JETPACK_PHYSICAL_HEIGHT 12
#define JETPACK_PHYSICAL_WIDTH 14

#define JETPACK_SPRITE_HEIGHT 16
#define JETPACK_SPRITE_WIDTH 16
#define JETPACK_SPRITE_SIZE JETPACK_SPRITE_WIDTH * JETPACK_SPRITE_HEIGHT

#define MAX_X_SPEED_JETPACK 4
#define MAX_Y_SPEED_JETPACK 4

//http://spelunky.wikia.com/wiki/jetpack
class Jetpack : public MovingObject, public ShoppingObject {

public:

    void introduce_yourself() override { printf("JETPACK\n"); };

    ~Jetpack() override;

    Jetpack();

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

    FallPoof *poofs[2]{};

    u8 *frameGfx{};

    int poof_spawn_timer{};

    void set_position();

    void equip();
};


#endif //SPELUNKYDS_JETPACK_H
