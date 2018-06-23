//
// Created by xdbeef on 18.05.18.
//

#ifndef SPELUNKYDS_PISTOL_H
#define SPELUNKYDS_PISTOL_H

#include "../moving_object.h"
#include "../sprite_state.h"
#include "../shopping_object.h"

#define PISTOL_PHYSICAL_HEIGHT 6
#define PISTOL_PHYSICAL_WIDTH 9
#define PISTOL_SPRITE_HEIGHT 16
#define PISTOL_SPRITE_WIDTH 16

#define MAX_X_SPEED_PISTOL 4
#define MAX_Y_SPEED_PISTOL 4


class Pistol : public MovingObject, public ShoppingObject {

public:

    Pistol();

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

    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    SpriteInfo *blast_mainSpriteInfo{};
    SpriteInfo *blast_subSpriteInfo{};

    int cooldown{};
    bool firing{};
    int animFrame{};
    int animFrameTimer{};

    u8 *frameGfx{};

    void set_position();

    void equip();
};


#endif //SPELUNKYDS_PISTOL_H
