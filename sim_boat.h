#ifndef SIM_BOAT_H_
#define SIM_BOAT_H_

struct _boat;

typedef struct _boat {
    double x; // x coordinate of the boat
    double y; // y coordinate of the boat
    double angle; // orientation of the boat
    double sail_angle;
    double rudder_angle;
} Boat;

#endif
