#ifndef SAIL_PHYSICS_H_
#define SAIL_PHYSICS_H_

#include "sail_boat.h"
#include "sail_wind.h"

void sail_physics_update(Boat *boat, Wind *wind, const double dt);

#endif
