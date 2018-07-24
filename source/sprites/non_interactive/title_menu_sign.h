//
// Created by xdbeef on 30.04.18.
//

#ifndef SPELUNKYDS_TITLEMENUSIGN_H
#define SPELUNKYDS_TITLEMENUSIGN_H

#define TUTORIAL_QUIT_SIGN_SPRITE_WIDTH 16
#define TUTORIAL_QUIT_SIGN_SPRITE_HEIGHT 16

#define START_SCORES_SIGN_SPRITE_WIDTH 32
#define START_SCORES_SIGN_SPRITE_HEIGHT 32

#include "../moving_object.h"
#include "../menu_sign_type.hpp"

//TODO SPLIT THIS TO SEPARATE CLASSES

//used for main menu signs: tutorial, scores, start, quit
class TitleMenuSign : public MovingObject {

public:

    TitleMenuSign();

    void initSprite() override;

    void updateOther() override {};

    void init() override;

    void draw() override;

    void apply_dmg(int dmg_to_apply) override {};

    void updateTimers() override {};

    void update_position() override {};

    void updateSpeed() override {};

    void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) override {};

    void updateCollisionsOtherMoving() override {};

    void onCollisionWithMainCharacter() override {};

    SpriteInfo *mainSpriteInfo_1{};
    SpriteInfo *mainSpriteInfo_2{};

    SpriteInfo *subSpriteInfo_1{};
    SpriteInfo *subSpriteInfo_2{};

    OamType oamType{};
    MenuSignType menuSignType{};
};


#endif //SPELUNKYDS_TITLEMENUSIGN_H
