//
// Created by xdbeef on 07.04.18.
// sizeof(BaseCreature) is 805 bytes
//

#ifndef SPELUNKYDS_MOVINGOBJECT_H
#define SPELUNKYDS_MOVINGOBJECT_H

#include "spritesheet_type.hpp"
#include "sprite_state.hpp"
#include "sprite_type.hpp"
#include "../interfaces/IRenderable.h"
#include "../interfaces/ICollidable.h"

/**
 *  https://en.wikibooks.org/wiki/C%2B%2B_Programming/Classes/Abstract_Classes
 *  Quote:
 *  The concept of interface is mapped to pure abstract classes in C++, as there is no "interface"
 *  construct in C++ the same way that there is in Java.
 *  https://en.wikibooks.org/wiki/C%2B%2B_Programming/Classes#Virtual_member_functions
 *  https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean
 */
class BaseCreature : public IRenderable, public ICollidable {

public:

    // Constructor of inheriting class should call init_sprites.
    BaseCreature(
            int x,
            int y,
            const u16 sprite_width,
            const u16 sprite_height,
            const SpritesheetType spritesheet_type,
            u16 physical_width,
            u16 physical_height
    ) :
            IRenderable(sprite_width, sprite_height, spritesheet_type),
            ICollidable(physical_width, physical_height) {
        set_xy(x, y);
    }

    virtual ~BaseCreature() = default;

    inline void update() {
        if (can_update_collidable()) update_collidable();
        update_creature_specific();
    };

    //for debugging purposes
    //https://stackoverflow.com/questions/3649278/how-can-i-get-the-class-name-from-a-c-object
    //https://stackoverflow.com/questions/32016809/using-typeid-to-get-name-of-derived-class
    virtual void introduce_yourself() = 0;

    virtual void update_creature_specific() = 0;

    virtual void apply_dmg(int dmg_to_apply) = 0;

    virtual bool can_update_collidable() = 0;

    int hitpoints{};
    bool standingOnLeftEdge{};
    bool standingOnRightEdge{};
    bool hold_by_main_dude{};
    bool activated{}; //ex. shooting shotgun, or arming bomb
    bool killed{};

    SpriteState sprite_state{};
    SpriteType sprite_type{};

    // TODO Distribute these utils between ICollidable/IRenderable, some may stay here as they are creature-specific.
    // Some can be moved to other class / namespace, like spawn_blood.

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
