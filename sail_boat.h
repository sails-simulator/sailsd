#ifndef SAIL_BOAT_H_
#define SAIL_BOAT_H_

#include <librsvg/rsvg.h>

typedef struct _images {
    RsvgHandle *hull;
    RsvgHandle *sail;
    RsvgHandle *rudder;
    RsvgDimensionData *hull_dimensions;
    RsvgDimensionData *sail_dimensions;
    RsvgDimensionData *rudder_dimensions;
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

void sail_boat_draw(cairo_t *cr, Boat *boat);

Boat* sail_boat_new();
void sail_boat_free(Boat *boat);

#endif
