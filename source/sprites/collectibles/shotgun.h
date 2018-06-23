//
// Created by xdbeef on 17.05.18.
//

#ifndef SPELUNKYDS_SHOTGUN_H
#define SPELUNKYDS_SHOTGUN_H


#include "../moving_object.h"
#include "../shopping_object.h"
#include "../animations/blast.h"

#define SHOTGUN_PHYSICAL_HEIGHT 7
#define SHOTGUN_PHYSICAL_WIDTH 13
#define SHOTGUN_SPRITE_HEIGHT 16
#define SHOTGUN_SPRITE_WIDTH 16

#define MAX_X_SPEED_SHOTGUN 4
#define MAX_Y_SPEED_SHOTGUN 4


class Shotgun: public MovingObject, public ShoppingObject {

public:

    Shotgun();

    void updateOther() override {};

    void init() override;

    void draw() override;

    void initSprite() override;

    void apply_dmg(int dmg_to_apply) override {};

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    bool hold_by_anyone{};

    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};

    Blast *blast{};

    int cooldown{};
    bool firing{};
    u8 * frameGfx{};

    void set_position();

    void spawn_bullets();

    void equip();
};






#endif //SPELUNKYDS_SHOTGUN_H
