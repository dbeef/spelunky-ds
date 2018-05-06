//
// Created by xdbeef on 07.04.18.
//

#ifndef SPELUNKYDS_MOVINGOBJECT_H
#define SPELUNKYDS_MOVINGOBJECT_H

#include "../memory/oam_manager.h"
#include "../camera/camera.h"
#include "spritesheet_type.h"

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

//    MovingObject(int p_physical_width, int p_physical_height, int p_sprite_width, int p_sprite_height);

    void update() {
        updateOther();
        updateSpeed();
        updateTimers();
    };

    void limit_speed(int max_x, int max_y);

    void apply_friction(int friction_delta_time_ms, int friction_delta_speed);

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

    //for rendering
    int sprite_width;
    int sprite_height;

    //for collisions
    int physical_width;
    int physical_height;

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

    int friction_timer;

    SpritesheetType spriteType;
};


#endif //SPELUNKYDS_MOVINGOBJECT_H
