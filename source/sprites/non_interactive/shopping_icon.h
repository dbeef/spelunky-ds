//
// Created by xdbeef on 17.06.18.
//

#ifndef SPELUNKYDS_SHOPPING_ICON_H
#define SPELUNKYDS_SHOPPING_ICON_H


#define SHOPPING_ICON_SPRITE_HEIGHT 16
#define SHOPPING_ICON_SPRITE_WIDTH 16
#define SHOPPING_ICON_SPRITE_SIZE SHOPPING_ICON_SPRITE_WIDTH * SHOPPING_ICON_SPRITE_HEIGHT

#define SHOPPING_ICON_PHYSICAL_HEIGHT 6
#define SHOPPING_ICON_PHYSICAL_WIDTH 8

#include "../moving_object.h"

class ShoppingIcon : public MovingObject {

public:

    void introduce_yourself() override { printf("SHOPPING_ICON\n"); };

    ShoppingIcon();

    void set_ready_to_dispose();

    void initSprite() override;

    void deleteSprite() override;

    void updateOther() override {};

    void apply_dmg(int dmg_to_apply) override {};

    void init() override;

    void draw() override;

    void updateTimers() override {};

    void update_position() override {};

    void updateSpeed() override {};

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override {};

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};

    int anim_frame{};
    double anim_frame_timer{};
    double trigger_timer{};

    void set_position();

    void match_animation();
};




#endif //SPELUNKYDS_SHOPPING_ICON_H
