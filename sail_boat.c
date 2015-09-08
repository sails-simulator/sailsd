#include <stdlib.h>
#include <math.h>

#include "sail_boat.h"

double sail_boat_get_angle(const Boat *boat) {
    return boat->angle;
}

double sail_boat_get_rudder_angle(const Boat *boat) {
    return boat->rudder_angle;
}

double sail_boat_get_sail_angle(const Boat *boat) {
    return boat->sail_angle;
}

double sail_boat_get_velocity(const Boat *boat) {
    return boat->v;
}

Boat* sail_boat_new() {
    Boat *new_boat = malloc(sizeof(Boat));

    new_boat->x = 0;
    new_boat->y = 0;
    new_boat->angle = M_PI_4;
    new_boat->sail_angle = 0;
    new_boat->rudder_angle = 0.2;
    new_boat->sail_is_free = 0;

    new_boat->v = 5;
    new_boat->sheet_length = 0;
    new_boat->rotational_velocity = 0.0;
    new_boat->inertia = 10000.0;
    new_boat->drift_coefficient = 0.05;
    new_boat->rudder_distance = 4.0;
    new_boat->angular_friction = 6000;
    new_boat->mass = 300.0;
    new_boat->tangential_friction = 0.2;
    new_boat->mast_distance = 1;
    new_boat->rudder_lift = 2000.0;
    new_boat->sail_center_of_effort = 1.0;
    new_boat->sail_lift = 1000.0;

    return new_boat;
}

void sail_boat_free(Boat *boat) {
    free(boat);
}
