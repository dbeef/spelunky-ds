//
// Created by xdbeef on 29.12.18.
//

#ifndef SPELUNKYDS_ICOLLIDABLE_H
#define SPELUNKYDS_ICOLLIDABLE_H

#include <nds.h>

#include "IBase.h"
#include "../tiles/MapTile.hpp"

// Collidable provides fields and functions neccessary to:
// * check collisions between two Collidable objects
// * check collisions between map tiles
// * update xy position depending on xy speed
// * update xy speed depending on friction/gravity
class ICollidable : public virtual IBase {

public:

    static constexpr double default_max_x_speed = 3.0f;
    static constexpr double default_max_y_speed = 3.5f;
    static constexpr double default_friction = 0.055f;
    static constexpr float default_gravity = 0.19f;
    static constexpr float default_bouncing_factor_x = 0.15f;
    static constexpr float default_bouncing_factor_y = 0.35f;
    static constexpr u16 default_pos_update_delta = 15;

    ICollidable(
            u16 physical_width,
            u16 physical_height,
            double x_speed = 0,
            double y_speed = 0,
            double max_x_speed = default_max_x_speed,
            double max_y_speed = default_max_y_speed,
            double friction = default_friction
    ) :
            _physical_width(physical_width),
            _physical_height(physical_height),
            _bouncing_factor_x(default_bouncing_factor_x),
            _bouncing_factor_y(default_bouncing_factor_y),
            _max_x_speed(max_x_speed),
            _max_y_speed(max_y_speed),
            _friction(friction),
            _pos_update_delta(default_pos_update_delta),
            _x_speed(x_speed),
            _y_speed(y_speed),
            _gravity(default_gravity) {
        // do nothing
    }

    virtual ~ICollidable() = default;

    virtual bool can_apply_friction() const = 0;

    virtual bool can_apply_gravity() const = 0;

    // updated every _pos_update_delta overflow.
    MapTile *_neighboring_tiles[9]{};

    u16 _physical_width;
    u16 _physical_height;

    float _bouncing_factor_x;
    float _bouncing_factor_y;

    double _max_x_speed;
    double _max_y_speed;
    double _friction;

    double _pos_update_delta;
    double _x_speed;
    double _y_speed;
    double _pos_inc_timer{};

    bool _bottom_collision{};
    bool _upper_collision{};
    bool _left_collision{};
    bool _right_collision{};

    int _current_x_in_tiles{};
    int _current_y_in_tiles{};
    bool _map_collisions_checked{};

    float _gravity;

    // Updates moving timer, if overflows value, then updates speed & xy positions, and checking collisions.
    // Should be called every frame.
    void update_collidable();

    // Updates bottom/upper/left/right collision flags and xy speed.
    // public access since must be called on every creature when bomb explodes,
    // so things would fall if there's no more ground under them.
    // Updates _neighboring_tiles.
    void update_collisions_with_map(int x_current_pos_in_tiles, int y_current_pos_in_tiles);

    // TODO Some enum to specify what can be killed (creatures / jars)

    // I.e rocks/jars can kill creatures, but no jars
    bool kill_creatures_if_have_speed(u8 dmg_to_apply) const;

    // I.e bullets, they can harm both creatures and jars
    bool kill_creatures_jars(u8 dmg_to_apply) const;

    bool kill_main_dude_if_have_speed(u8 dmg_to_apply) const;

    void deal_damage_main_dude_on_collision(int dmg_to_apply) const;

private:

    // True if two collidables overlap.
    bool check_collision(ICollidable const &other_collidable) const;

    // Changes xy position according to current xy speed, checks collisions with map every moved pixel.
    void update_position();

    // Limits speed to max xy speeds.
    void limit_speed();

    // Removes x speed value according to friction variable, whether in contact with ground or not.
    void apply_friction();

    // Removes y speed value according to gravity variable, if not in contact with ground.
    void apply_gravity();

};


#endif //SPELUNKYDS_ICOLLIDABLE_H
