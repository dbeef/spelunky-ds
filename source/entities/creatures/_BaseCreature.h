//
// Created by xdbeef on 07.04.18.
//

#ifndef SPELUNKYDS_MOVINGOBJECT_H
#define SPELUNKYDS_MOVINGOBJECT_H


#include "../_common/Orientation.hpp"
#include "CreatureType.hpp"
#include "../_interfaces/IRenderable.h"
#include "../_interfaces/ICollidable.h"
#include "../items/_BaseItem.h"
#include "../../graphics/SpritesheetType.hpp"

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
            u16 physical_height,
            CreatureType creature_type
    );

    virtual ~BaseCreature() = default;

    inline void update() {
        if (can_update_collidable()) update_collidable();
        update_creature_specific();
    };

    virtual void update_creature_specific() = 0;

    virtual void apply_dmg(int dmg_to_apply) = 0;

    virtual bool can_update_collidable() const = 0;

    int hitpoints{};
    bool standingOnLeftEdge{};
    bool standingOnRightEdge{};
    bool hold_by_main_dude{};
    bool activated{}; //ex. shooting shotgun, or arming bomb
    bool killed{};

    Orientation sprite_state{};
    const CreatureType _creature_type;


    void spawn_blood() const;

    void kill_if_main_dude_jumped_on_you(int dmg_to_apply);

    void kill_if_whip(int dmg_to_apply);



    bool check_if_can_be_opened();

    void check_if_can_be_pickuped();

    bool check_if_can_be_equipped();



    void set_pickuped_position(int pickup_offset_x, int pickup_offset_y);

    void set_pickuped_position(int pickup_offset_x_left, int pickup_offset_x_right, int pickup_offset_y);

    void set_pickuped_position_not_checking(int pickup_offset_x, int pickup_offset_y);

    void set_pickuped_position_on_another_moving_obj(int pickup_offset_x, int pickup_offset_y, BaseCreature *m) const;

    void set_pickuped_position_on_another_moving_obj(int pickup_offset_x, int pickup_offset_y, BaseItem *m) const;
};

#endif //SPELUNKYDS_MOVINGOBJECT_H
