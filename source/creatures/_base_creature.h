//
// Created by xdbeef on 07.04.18.
// sizeof(BaseCreature) is 805 bytes
//

#ifndef SPELUNKYDS_MOVINGOBJECT_H
#define SPELUNKYDS_MOVINGOBJECT_H

#include <cstdio>

#include "../memory/oam_manager.hpp"
#include "../camera/camera.hpp"
#include "spritesheet_type.hpp"
#include "sprite_state.hpp"
#include "sprite_type.hpp"

/**
 *  https://en.wikibooks.org/wiki/C%2B%2B_Programming/Classes/Abstract_Classes
 *  Quote:
 *  The concept of interface is mapped to pure abstract classes in C++, as there is no "interface"
 *  construct in C++ the same way that there is in Java.
 *  https://en.wikibooks.org/wiki/C%2B%2B_Programming/Classes#Virtual_member_functions
 *  https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean
 */
class BaseCreature {
public:

    //for debugging purposes
    //https://stackoverflow.com/questions/3649278/how-can-i-get-the-class-name-from-a-c-object
    //https://stackoverflow.com/questions/32016809/using-typeid-to-get-name-of-derived-class
    virtual void introduce_yourself() = 0;

    virtual ~BaseCreature() = default;

    void update() {
        updateOther();
        updateSpeed();
        updateTimers();
    };

    void set_sprite_state_basing_on_speed();

    void limit_speed(int max_x, int max_y);

    void apply_friction(float friction_delta_speed);

    virtual void initSprite() = 0;

    virtual void deleteSprite() = 0;

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
    bool activated{}; //ex. shooting shotgun, or arming bomb

    bool killed{};
    bool ready_to_dispose{};

    SpriteState sprite_state{};

    SpritesheetType spritesheet_type{};

    SpriteType sprite_type{};

    //todo apply this
    double speed_of_throwing_x{};
    double speed_of_throwing_y{};

    void get_x_y_viewported(int *out_main_x, int *out_main_y, int *out_sub_x, int *out_sub_y);

    void apply_gravity(double acceleration);

    bool kill_mobs_if_thrown(int dmg_to_apply);

    void check_if_can_be_pickuped();

    void set_pickuped_position(int pickup_offset_x, int pickup_offset_y);

    void set_pickuped_position(int pickup_offset_x_left, int pickup_offset_x_right, int pickup_offset_y);

    bool check_if_can_be_opened();

    bool kill_mobs_items_if_thrown(int dmg_to_apply);

    bool check_if_can_be_equipped();

    void kill_if_main_dude_jumped_on_you(int dmg_to_apply);

    void kill_if_whip(int dmg_to_apply);

    void deal_damage_main_dude_on_collision(int dmg_to_apply);

    void spawn_blood();

    void set_pickuped_position_not_checking(int pickup_offset_x, int pickup_offset_y);

    bool kill_main_dude_if_thrown(int dmg_to_apply);

    void set_pickuped_position_on_another_moving_obj(int pickup_offset_x, int pickup_offset_y, BaseCreature *m);
};

#endif //SPELUNKYDS_MOVINGOBJECT_H
