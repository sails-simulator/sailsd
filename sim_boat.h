#ifndef SIM_BOAT_H_
#define SIM_BOAT_H_

#include <librsvg/rsvg.h>

typedef struct _images {
    RsvgHandle *hull;
} SVGImages;

struct _boat;

typedef struct _boat {
    double x; // x coordinate of the boat
    double y; // y coordinate of the boat
    double angle; // orientation of the boat
    double sail_angle;
    double rudder_angle;

    SVGImages* images;
} Boat;

void sim_boat_draw(cairo_t *cr, Boat *boat);

Boat* sim_boat_new();
void sim_boat_free(Boat *boat);

#endif
