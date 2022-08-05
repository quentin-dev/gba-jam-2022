#ifndef GJ_COLLISIONS_H
#define GJ_COLLISIONS_H

#include "bn_log.h"
#include "bn_point.h"

#include "gj_constants.h"

namespace gj::collisions {

int get_collision_index(bn::point *position, int sprites_per_row) {

  return position->y() * sprites_per_row + position->x();
}

bool tile_is_walkable(int index, const uint8_t *collision_map) {
  BN_LOG("Attempting to get index: ", index);
  return collision_map[index] == gj::constants::Collisions::WALKABLE;
}

} // namespace gj::collisions

#endif