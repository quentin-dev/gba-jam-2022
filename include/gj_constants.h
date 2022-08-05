#ifndef GJ_CONSTANTS_H
#define GJ_CONSTANTS_H

namespace gj::constants {

const auto PLAYER_MOVEMENT_INCREMENT = 2;
const auto BG_MOVEMENT_INCREMENT = 16.f;

const auto COOLDOWN_FRAMES = 12;

enum Collisions { NON_WALKABLE = 0, WALKABLE = 1 };

enum Directions { UP, DOWN, LEFT, RIGHT };

} // namespace gj::constants

#endif