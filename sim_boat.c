#include <stdlib.h>
#include <cairo.h>
#include <librsvg/rsvg.h>

#include "sim_boat.h"

void sim_boat_draw(cairo_t *cr, Boat *boat) {

}

Boat* sim_boat_new() {
    Boat *new_boat = malloc(sizeof(Boat));

    new_boat->x = 0;
    new_boat->y = 0;
    new_boat->angle = 0;
    new_boat->sail_angle = 0;
    new_boat->rudder_angle = 0;

    return new_boat;
}
