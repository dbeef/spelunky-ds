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

    void update() {
        updateOther();
        updateSpeed();
        updateTimers();
    };

    void limit_speed(int max_x, int max_y);

    void apply_friction(float friction_delta_speed);

    virtual void initSprite() = 0;

    virtual void draw() = 0;

    virtual void apply_dmg(int dmg_to_apply) = 0;

    virtual void init() = 0;

    virtual void updateTimers() = 0;

    virtual void update_position();

    virtual void updateSpeed() = 0;

    virtual void updateOther() = 0;

    virtual void updateCollisionsMap(int x_current_pos_in_tiles, int y_current_pos_in_tiles) = 0;

    virtual void updateCollisionsOtherMoving() = 0;

    virtual void onCollisionWithMainCharacter() = 0;

    //TODO Make x,y as float, cast (floor) to int when drawn.
    int x{};
    int y{};

    int hitpoints{};

    //for rendering
    int sprite_width{};
    int sprite_height{};

    //for collisions
    int physical_width{};
    int physical_height{};

    double xSpeed{};
    double ySpeed{};

    bool standingOnLeftEdge{};
    bool standingOnRightEdge{};
    bool bottomCollision{};
    bool upperCollision{};
    bool leftCollision{};
    bool rightCollision{};

    bool hold_by_main_dude{};
    bool activated_by_main_dude{}; //ex. shooting shotgun, or arming bomb

    bool killed{};
    bool ready_to_dispose{};

    SpritesheetType spriteType{};

    void get_x_y_viewported(int *out_main_x, int *out_main_y, int *out_sub_x, int *out_sub_y);

    void apply_gravity(double acceleration);

    bool kill_mobs_if_thrown(int dmg_to_apply);

    void check_if_can_be_pickuped();

    void set_pickuped_position(int pickup_offset_x, int pickup_offset_y);

    bool check_if_can_be_opened();

    bool kill_mobs_items_if_thrown(int dmg_to_apply);

    bool check_if_can_be_equipped();

    void kill_if_main_dude_jumped_on_you(int dmg_to_apply);

    void kill_if_whip(int dmg_to_apply);

    void deal_damage_main_dude_on_collision(int dmg_to_apply);

    void spawn_blood();

    void set_pickuped_position_not_checking(int pickup_offset_x, int pickup_offset_y);
};


#endif //SPELUNKYDS_MOVINGOBJECT_H
