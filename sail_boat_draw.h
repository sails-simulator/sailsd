#include "sail_boat.h"

typedef struct _images {
    RsvgHandle *hull;
    RsvgHandle *sail;
    RsvgHandle *rudder;
    RsvgDimensionData *hull_dimensions;
    RsvgDimensionData *sail_dimensions;
    RsvgDimensionData *rudder_dimensions;
} SVGImages;

void sail_boat_draw(cairo_t *cr, const Boat *boat, const SVGImages *images);