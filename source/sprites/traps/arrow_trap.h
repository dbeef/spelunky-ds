//
// Created by xdbeef on 23.06.18.
//

#ifndef SPELUNKYDS_ARROW_TRAP_H
#define SPELUNKYDS_ARROW_TRAP_H

#include "../moving_object.h"

#define ARROW_TRAP_PHYSICAL_HEIGHT 16
#define ARROW_TRAP_PHYSICAL_WIDTH 16
#define ARROW_TRAP_SPRITE_HEIGHT 16
#define ARROW_TRAP_SPRITE_WIDTH 16

//http://spelunky.wikia.com/wiki/Arrow_Trap
class ArrowTrap : public MovingObject {

public:

    void introduce_yourself() override { printf("ARROW_TRAP\n"); };

    ArrowTrap();

    void updateOther() override {};

    void init() override;

    void draw() override;

    void initSprite() override;

    void deleteSprite() override;

    void apply_dmg(int dmg_to_apply) override {};

    void updateTimers() override {};

    void updateSpeed() override {};

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override {};

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};

    void spawn_arrow();
};


#endif //SPELUNKYDS_ARROW_TRAP_H
