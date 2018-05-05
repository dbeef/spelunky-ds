//
// Created by xdbeef on 07.04.18.
//

#ifndef SPELUNKYDS_MOVINGOBJECT_H
#define SPELUNKYDS_MOVINGOBJECT_H

#include "../memory/OAMManager.h"
#include "../camera/Camera.h"
#include "SpritesheetTypes.h"

/**
 *  https://en.wikibooks.org/wiki/C%2B%2B_Programming/Classes/Abstract_Classes
 *  Quote:
 *  The concept of interface is mapped to pure abstract classes in C++, as there is no "interface"
 *  construct in C++ the same way that there is in Java.
 *  https://en.wikibooks.org/wiki/C%2B%2B_Programming/Classes#Virtual_member_functions
 *  https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean
 */
class MovingObject {
public:

    //todo virtual void for Oam init

    void update() {
        updateOther();
        updateSpeed();
        updateTimers();
    };


    virtual void initSprite() = 0;

    virtual void draw() = 0;

    virtual void kill() = 0;

    virtual void init() = 0;

    virtual void updateTimers() = 0;

    virtual void updatePosition() = 0;

    virtual void updateSpeed() = 0;

    virtual void updateOther() = 0;

    virtual void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) = 0;

    virtual void updateCollisionsOtherMoving() = 0;

    virtual void onCollisionWithMainCharacter() = 0;

    //TODO Make x,y as float, cast (floor) to int when drawn.
    int x;
    int y;

    double xSpeed;
    double ySpeed;

    bool standingOnLeftEdge;
    bool standingOnRightEdge;
    bool bottomCollision;
    bool upperCollision;
    bool leftCollision;
    bool rightCollision;

    bool hold_by_main_dude;
    bool activated_by_main_dude; //ex. shooting shotgun, or arming bomb

    bool killed;
    bool ready_to_dispose;

    SpritesheetType spriteType;
};


#endif //SPELUNKYDS_MOVINGOBJECT_H
