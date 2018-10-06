//
// Created by xdbeef on 02.05.18.
// http://spelunky.wikia.com/wiki/Bat
//

#ifndef SPELUNKYDS_BAT_H
#define SPELUNKYDS_BAT_H

#define MAX_X_SPEED_BAT 4
#define MAX_Y_SPEED_BAT 4

#define BAT_PHYSICAL_HEIGHT 16
#define BAT_PHYSICAL_WIDTH 16

#define BAT_SPRITE_HEIGHT 16
#define BAT_SPRITE_WIDTH 16
#define BAT_SPRITE_SIZE BAT_SPRITE_HEIGHT * BAT_SPRITE_WIDTH

#include "../moving_object.h"
#include "../sprite_state.hpp"
#include "../sprite_info.h"

//http://spelunky.wikia.com/wiki/Bat
class Bat : public MovingObject {

public:

    void introduce_yourself() override { printf("BAT\n"); };

    Bat();

    Bat(int x, int y);

    void updateOther() override {};

    void init() override;

    void draw() override;

    void apply_dmg(int dmg_to_apply) override ;

    void updateTimers() override {};

    void updateSpeed() override;

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override;

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    void initSprite() override;

    void deleteSprite() override;

    double pos_inc_timer{};

    SpriteInfo *mainSpriteInfo {};
    SpriteInfo *subSpriteInfo {};
    u8 *frameGfx{};

    int animFrame{};
    double animFrameTimer{};

    bool hanging{};
    bool hunting{};

    void set_sprite_hanging();

    void set_sprite_flying_right();

    void set_sprite_flying_left();

    void set_position();

    void follow_main_dude();

    void match_animation();
};



#endif //SPELUNKYDS_BAT_H
