// System includes
#include "bn_affine_bg_ptr.h"
#include "bn_assert.h"
#include "bn_bg_palette_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_blending.h"
#include "bn_core.h"
#include "bn_fixed_rect.h"
#include "bn_keypad.h"
#include "bn_log.h"
#include "bn_math.h"
#include "bn_optional.h"
#include "bn_point.h"
#include "bn_rect_window.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_vector.h"
#include "bn_window.h"

// Project includes
#include "gj_bg_islands_collisions.hh"
#include "gj_bg_overworld_small_collisions.hh"
#include "gj_big_sprite_font.h"
#include "gj_collisions.h"
#include "gj_constants.h"
#include "gj_movement.h"

// Build includes
#include "bn_bg_palette_items_text_bg_palette.h"
#include "bn_regular_bg_items_overworld_small.h"
#include "bn_regular_bg_items_text_bg.h"
#include "bn_sprite_items_player_old.h"

int main() {
  bn::core::init();

  bn::regular_bg_ptr bg_text = bn::regular_bg_items::text_bg.create_bg(0, 0);
  bg_text.set_visible(true);
  bg_text.set_priority(0);

  bn::window outside_window = bn::window::outside();
  outside_window.set_show_all();
  outside_window.set_show_bg(bg_text, false);

  bn::blending::set_transparency_alpha(0.90);
  bg_text.set_blending_enabled(true);

  bn::fixed_rect text_rect = bn::fixed_rect(0, 0, 240, 160);

  bn::rect_window text_window = bn::rect_window::internal();
  text_window.set_visible(false);
  text_window.set_boundaries(text_rect);

  bn::regular_bg_ptr bg_islands =
      bn::regular_bg_items::overworld_small.create_bg(8, -8);
  bg_islands.set_priority(1);

  bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));

  bn::sprite_ptr player_old_sprite =
      bn::sprite_items::player_old.create_sprite(0, 0);

  player_old_sprite.set_bg_priority(1);

  // bn::sprite_text_generator text_generator(gj::fixed_32x64_sprite_font);

  // text_generator.set_center_alignment();
  // bn::vector<bn::sprite_ptr, 32> text_sprites;

  // text_generator.generate(0, 52, "Hello, World!", text_sprites);

  bn::fixed movement_origin = 0;

  bn::optional<int> frames_to_wait;

  bn::optional<gj::constants::Directions> movement_direction;
  bn::optional<bn::point> next_position;
  bn::point player_position(15, 16);

  const uint8_t *current_collision_map = gj::overworld_small::collisions;
  const uint8_t current_sprites_per_row = gj::overworld_small::SPRITES_PER_ROW;
  const uint8_t current_sprites_per_column =
      gj::overworld_small::SPRITES_PER_COLUMN;

  while (true) {

    if (frames_to_wait.has_value() && frames_to_wait.value() == 0) {

      if (movement_direction.value() == gj::constants::Directions::UP ||
          movement_direction.value() == gj::constants::Directions::DOWN) {
        if (abs(bg_islands.y() - movement_origin) <
            gj::constants::BG_MOVEMENT_INCREMENT) {
          if (movement_direction.value() == gj::constants::Directions::UP) {
            bg_islands.set_y((movement_origin.integer() +
                              gj::constants::BG_MOVEMENT_INCREMENT));
          } else if (movement_direction.value() ==
                     gj::constants::Directions::DOWN) {
            bg_islands.set_y((movement_origin.integer() -
                              gj::constants::BG_MOVEMENT_INCREMENT));
          }
        }
      } else if (movement_direction.value() ==
                     gj::constants::Directions::LEFT ||
                 movement_direction.value() ==
                     gj::constants::Directions::RIGHT) {
        if (abs(bg_islands.x() - movement_origin) <
            gj::constants::BG_MOVEMENT_INCREMENT) {
          if (movement_direction.value() == gj::constants::Directions::LEFT) {
            bg_islands.set_x((movement_origin.integer() +
                              gj::constants::BG_MOVEMENT_INCREMENT));
          } else if (movement_direction.value() ==
                     gj::constants::Directions::RIGHT) {
            bg_islands.set_x((movement_origin.integer() -
                              gj::constants::BG_MOVEMENT_INCREMENT));
          }
        }
      }

      BN_ASSERT(bg_islands.y().ceil_integer() == bg_islands.y().floor_integer(),
                "BG Islands - Y position is not an int: ", bg_islands.y());

      BN_LOG("BG Islands - X: ", bg_islands.x());
      BN_LOG("BG Islands - Y: ", bg_islands.y());

      movement_direction.reset();
      next_position.reset();
      frames_to_wait.reset();

      BN_LOG("FRAMES TO WAIT: RESET");
    }

    if (!frames_to_wait.has_value()) {
      if (bn::keypad::a_pressed()) {
        text_window.set_visible(!text_window.visible());
        // bg_text.set_visible(!bg_text.visible());
      }

      if (bn::keypad::up_held()) {

        next_position = gj::movement::get_next_player_position(
            &player_position, gj::constants::Directions::UP,
            current_sprites_per_row, current_sprites_per_column);
        int new_ndx = gj::collisions::get_collision_index(
            &next_position.value(), current_sprites_per_row);

        if (gj::collisions::tile_is_walkable(new_ndx, current_collision_map)) {
          movement_direction = gj::constants::Directions::UP;
          frames_to_wait = gj::constants::COOLDOWN_FRAMES;
          movement_origin = bg_islands.y();
          player_position.set_y(next_position.value().y());
        }

      } else if (bn::keypad::down_held()) {

        next_position = gj::movement::get_next_player_position(
            &player_position, gj::constants::Directions::DOWN,
            current_sprites_per_row, current_sprites_per_column);
        int new_ndx = gj::collisions::get_collision_index(
            &next_position.value(), current_sprites_per_row);

        if (gj::collisions::tile_is_walkable(new_ndx, current_collision_map)) {
          movement_direction = gj::constants::Directions::DOWN;
          frames_to_wait = gj::constants::COOLDOWN_FRAMES;
          movement_origin = bg_islands.y();
          player_position.set_y(next_position.value().y());
        }

      } else if (bn::keypad::left_held()) {

        next_position = gj::movement::get_next_player_position(
            &player_position, gj::constants::Directions::LEFT,
            current_sprites_per_row, current_sprites_per_column);
        int new_ndx = gj::collisions::get_collision_index(
            &next_position.value(), current_sprites_per_row);

        if (gj::collisions::tile_is_walkable(new_ndx, current_collision_map)) {
          movement_direction = gj::constants::Directions::LEFT;
          frames_to_wait = gj::constants::COOLDOWN_FRAMES;
          movement_origin = bg_islands.x();
          player_position.set_x(next_position.value().x());
        }

      } else if (bn::keypad::right_held()) {

        next_position = gj::movement::get_next_player_position(
            &player_position, gj::constants::Directions::RIGHT,
            current_sprites_per_row, current_sprites_per_column);
        int new_ndx = gj::collisions::get_collision_index(
            &next_position.value(), current_sprites_per_row);

        if (gj::collisions::tile_is_walkable(new_ndx, current_collision_map)) {
          movement_direction = gj::constants::Directions::RIGHT;
          frames_to_wait = gj::constants::COOLDOWN_FRAMES;
          movement_origin = bg_islands.x();
          player_position.set_x(next_position.value().x());
        }
      }
    }

    if (frames_to_wait.has_value()) {
      BN_LOG("FRAMES TO WAIT: ", frames_to_wait.value());
      auto to_add = (gj::constants::BG_MOVEMENT_INCREMENT /
                     gj::constants::COOLDOWN_FRAMES);

      if (movement_direction.value() == gj::constants::Directions::UP) {
        bg_islands.set_y((bg_islands.y() + to_add));
      } else if (movement_direction.value() ==
                 gj::constants::Directions::DOWN) {
        bg_islands.set_y((bg_islands.y() - to_add));
      } else if (movement_direction.value() ==
                 gj::constants::Directions::LEFT) {
        bg_islands.set_x((bg_islands.x() + to_add));
      } else if (movement_direction.value() ==
                 gj::constants::Directions::RIGHT) {
        bg_islands.set_x((bg_islands.x() - to_add));
      }

      frames_to_wait.value() -= 1;
    }

    bn::core::update();
  }
}
