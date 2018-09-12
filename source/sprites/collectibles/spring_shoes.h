//
// Created by xdbeef on 18.05.18.
//

#ifndef SPELUNKYDS_SPRING_SHOES_H
#define SPELUNKYDS_SPRING_SHOES_H


#include "../sprite_info.h"
#include "../sprite_state.hpp"
#include "../moving_object.h"
#include "../shopping_object.h"

#define SPRING_SHOES_PHYSICAL_HEIGHT 12
#define SPRING_SHOES_PHYSICAL_WIDTH 14

#define SPRING_SHOES_SPRITE_HEIGHT 16
#define SPRING_SHOES_SPRITE_WIDTH 16
#define SPRING_SHOES_SPRITE_SIZE SPRING_SHOES_SPRITE_WIDTH * SPRING_SHOES_SPRITE_HEIGHT

#define MAX_X_SPEED_SPRING_SHOES 4
#define MAX_Y_SPEED_SPRING_SHOES 4

class SpringShoes : public MovingObject , public ShoppingObject {

public:

    void introduce_yourself() override { printf("SPRING_SHOES\n"); };

    SpringShoes();

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

    bool collected{};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    u8 *frameGfx{};

    void set_position();

    void equip();
};


#endif //SPELUNKYDS_SPRING_SHOES_H
