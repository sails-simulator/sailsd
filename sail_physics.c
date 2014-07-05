#include "sail_physics.h"
#include "sail_boat.h"

void sail_physics_update(Boat *boat, double dt) {
    boat->angle = boat->angle + 0.1 * dt;
}
