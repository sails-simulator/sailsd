#ifndef SAIL_BOAT_H_
#define SAIL_BOAT_H_

struct _boat;

typedef struct _boat {
    double x, y; // coordinates of boat
    double angle; // orientation of boat
    double sail_angle;
    double rudder_angle;
    double sheet_length;
    int sail_is_free; // is the sail free to move?

    double v, rotational_velocity, ell; // state variables
    double drift_coefficient, mass, rudder_distance, mast_distance,
           rudder_lift, sail_lift, tangential_friction, angular_friction,
           sail_center_of_effort, inertia; // parameters
} Boat;

Boat* sail_boat_new();
void sail_boat_free(Boat *boat);

double sail_boat_get_angle(const Boat *boat);
double sail_boat_get_rudder_angle(const Boat *boat);
double sail_boat_get_sail_angle(const Boat *boat);
double sail_boat_get_velocity(const Boat *boat);

#endif
