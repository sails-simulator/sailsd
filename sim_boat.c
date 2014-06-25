#include <stdlib.h>
#include <glib.h>
#include <cairo.h>
#include <librsvg/rsvg.h>

#include "sim_boat.h"

static Boat* load_boat_images(Boat *boat) {
    GError *err = NULL;
    boat->images->hull = rsvg_handle_new_from_file("resources/hull.svg", &err);
    return boat;
}

void sim_boat_draw(cairo_t *cr, Boat *boat) {

}

Boat* sim_boat_new() {
    Boat *new_boat = malloc(sizeof(Boat));

    new_boat->x = 0;
    new_boat->y = 0;
    new_boat->angle = 0;
    new_boat->sail_angle = 0;
    new_boat->rudder_angle = 0;

    load_boat_images(new_boat);

    return new_boat;
}
