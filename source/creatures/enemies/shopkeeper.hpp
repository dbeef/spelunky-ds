//
// Created by xdbeef on 25.05.18.
//

#ifndef SPELUNKYDS_SHOPKEEPER_H
#define SPELUNKYDS_SHOPKEEPER_H

#define SHOPKEEPER_STUN_TIME 7000
#define SHOPKEEPER_HITPOINTS 3

#include "../_base_creature.h"
#include "../sprite_state.hpp"
#include "../sprite_info.h"
#include "../../interfaces/shopping_object.h"
#include "../collectibles/shotgun.hpp"

//http://spelunky.wikia.com/wiki/Shopkeeper
class Shopkeeper : public BaseCreature {

public:

    static constexpr u8 shopkeeper_sprite_width = 16;
    static constexpr u8 shopkeeper_sprite_height = 16;
    static constexpr u16 shopkeeper_physical_width = 16;
    static constexpr u16 shopkeeper_physical_height = 16;
    static constexpr SpritesheetType shopkeeper_spritesheet_type = SpritesheetType::SHOPKEEPER;

    Shopkeeper(int x, int y);

    // Base creature overrides

    void update_creature_specific() override;

    void introduce_yourself() override { printf("WHIP\n"); };

    void apply_dmg(int dmg_to_apply) override;

    void onCollisionWithMainCharacter() override {};

    // IRenderable overrides

    void init_sprites() override;

    void delete_sprites() override;

    void update_sprites_position() override;

    // ICollidable overrides

    bool can_update_collidable() override { return !hold_by_main_dude && !_ready_to_dispose; }

    bool can_apply_friction() override { return !hold_by_main_dude && _bottom_collision && (!triggered || killed || stunned); }

    bool can_apply_gravity() override { return !hold_by_main_dude; }

    // Other, creature specific

    void randomizeMovement();

    void apply_walking_sprites();

    void apply_stunned_sprites();

    void make_some_movement();

    void apply_stunned_carried_sprites();

    void apply_dead_carried_sprites();

    void apply_dead_sprites();

    void match_animation();

    void set_shop_bounds();

    void check_if_dude_in_shop_bounds();

    void de_shopify_all_items();

    void spawn_shotgun();

    void trigger();

    Shotgun *shotgun{};
    bool holding_shotgun{};
    ShoppingObject *shop_items[4]{};
    SpriteInfo *mainSpriteInfo{};
    SpriteInfo *subSpriteInfo{};
    u8 *frameGfx{};
    double waitTimer{};
    double go_timer{};
    int anim_frame{};
    double anim_frame_timer{};
    double invert_speed_timer{};
    double blood_spawn_timer{};
    bool no_shotgun{};
    bool landlocked{};
    bool triggered{};
    bool standby{};
    bool stunned{};
    double stunned_timer{};
    int shop_bounds_left_x_px{};
    int shop_bounds_right_x_px{};
    int shop_bounds_up_y_px{};
    int shop_bounds_down_y_px{};
    double jumping_timer{};
    int staying_on_ground{};
    bool introduced_shop_name{};
};


#endif //SPELUNKYDS_SHOPKEEPER_H
