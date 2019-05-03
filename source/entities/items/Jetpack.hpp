//
// Created by xdbeef on 02.06.18.
//

#ifndef SPELUNKYDS_JETPACK_H
#define SPELUNKYDS_JETPACK_H



#include "../_common/Orientation.hpp"
#include "../items/_BaseItem.h"
#include "../_interfaces/ShoppingObject.h"
#include "../animations/FallPoof.hpp"

//http://spelunky.wikia.com/wiki/jetpack
class Jetpack : public BaseItem, public ShoppingObject {

public:
    
    static constexpr int jetpack_x_pickuped_offset_left = 4;
    static constexpr int jetpack_x_pickuped_offset_right = 4;
    static constexpr int jetpack_y_pickuped_offset = -4;
    static constexpr u16 jetpack_cost = 16000;
    static constexpr const char *jetpack_name = "JETPACK";
    static constexpr u8 jetpack_sprite_width = 16;
    static constexpr u8 jetpack_sprite_height = 16;
    static constexpr u16 jetpack_physical_width = 14;
    static constexpr u16 jetpack_physical_height = 12;
    static constexpr SpritesheetType jetpack_spritesheet_type = SpritesheetType::BAT_JETPACK;

    Jetpack(int x, int y) : BaseItem(
            x,
            y,
            jetpack_sprite_width,
            jetpack_sprite_height,
            jetpack_spritesheet_type,
            jetpack_physical_width,
            jetpack_physical_height,
            jetpack_x_pickuped_offset_left,
            jetpack_x_pickuped_offset_right,
            jetpack_y_pickuped_offset
    ), ShoppingObject(jetpack_cost, jetpack_name) {
        init_sprites();
        init_anim_icon();
        update_anim_icon(x, y, _physical_width);
    }

    // BaseItem overrides

    void update_item_specific() override;

    void print_classname() const override;

    // IRenderable overrides

    void init_sprites() override;

    // Other, creature specific

    ~Jetpack();

    void equip();

    double _poof_spawn_timer{};
    bool collected{};
    FallPoof *_poofs[2]{};

};


#endif //SPELUNKYDS_JETPACK_H
