//
// Created by xdbeef on 18.05.18.
//

#ifndef SPELUNKYDS_PISTOL_H
#define SPELUNKYDS_PISTOL_H

#include "../_base_creature.h"
#include "../sprite_state.hpp"
#include "../shopping_object.h"
#include "../animations/blast.hpp"

#define PISTOL_PHYSICAL_HEIGHT 6
#define PISTOL_PHYSICAL_WIDTH 9

#define PISTOL_SPRITE_HEIGHT 16
#define PISTOL_SPRITE_WIDTH 16
#define PISTOL_SPRITE_SIZE PISTOL_SPRITE_WIDTH * PISTOL_SPRITE_HEIGHT

#define MAX_X_SPEED_PISTOL 4
#define MAX_Y_SPEED_PISTOL 4

//http://spelunky.wikia.com/wiki/Pistol
class Pistol : public BaseCreature, public ShoppingObject {

public:

    void introduce_yourself() override { printf("PISTOL\n"); };

    Pistol();

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

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    Blast *blast{};

    double cooldown{};
    bool firing{};
    int animFrame{};
    int animFrameTimer{};

    u8 *frameGfx{};

    void set_position();

    void equip();

    void spawn_bullet();

    void handle_shooting();

    void match_animation();
};


#endif //SPELUNKYDS_PISTOL_H
