#ifndef GJ_MOVEMENT_H
#define GJ_MOVEMENT_H

#include "bn_fixed.h"
#include "bn_log.h"
#include "bn_point.h"

#include "gj_constants.h"

// ATTN: Most code in this file assumes movement cannot(!!!) wrap around edges
namespace gj::movement {

bn::point get_next_player_position(bn::point *position,
                                   const gj::constants::Directions direction,
                                   const int sprites_per_row,
                                   const int sprites_per_column) {
  bn::point next_position(*position);

  if (direction == gj::constants::Directions::UP) {
    int new_y = position->y() != 0 ? position->y() - 1 : sprites_per_column - 1;
    next_position.set_y(new_y);
  } else if (direction == gj::constants::Directions::DOWN) {
    int new_y = (position->y() + 1) % sprites_per_column;
    next_position.set_y(new_y);
  } else if (direction == gj::constants::Directions::LEFT) {
    int new_x = position->x() != 0 ? position->x() - 1 : sprites_per_row - 1;
    next_position.set_x(new_x);
  } else if (direction == gj::constants::Directions::RIGHT) {
    int new_x = (position->x() + 1) % sprites_per_row;
    next_position.set_x(new_x);
  }

  return next_position;
}

// TODO: Implement and use these methods in main.cpp

void finish_bg_movement(bn::point *position,
                        const gj::constants::Directions direction,
                        bn::fixed movement_origin) {}

bn::point get_next_bg_position(const gj::constants::Directions direction,
                               bn::point *position) {
  auto to_add =
      (gj::constants::BG_MOVEMENT_INCREMENT / gj::constants::COOLDOWN_FRAMES);

  bn::point next_position(*position);

  if (direction == gj::constants::Directions::UP) {
    next_position.set_y((position->y() + to_add));
  } else if (direction == gj::constants::Directions::DOWN) {
    next_position.set_y((position->y() - to_add));
  } else if (direction == gj::constants::Directions::LEFT) {
    next_position.set_x((position->x() + to_add));
  } else if (direction == gj::constants::Directions::RIGHT) {
    next_position.set_x((position->x() - to_add));
  }

  return next_position;
}

} // namespace gj::movement

#endif