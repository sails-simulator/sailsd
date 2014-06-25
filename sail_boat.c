#include <stdlib.h>
#include <glib.h>
#include <cairo.h>
#include <librsvg/rsvg.h>

#include "sail_boat.h"

static Boat* load_boat_images(Boat *boat) {
    GError *err = NULL;
    boat->images = malloc(sizeof(SVGImages));
    boat->images->hull = rsvg_handle_new_from_file("resources/hull.svg", &err);
    if (err != NULL) {
        g_log("sail", G_LOG_LEVEL_CRITICAL, "can't open file: %s", err->message);
    }
    return boat;
}

void sail_boat_draw(Boat *boat, cairo_t *cr) {
    rsvg_handle_render_cairo(boat->images->hull, cr);
}

Boat* sail_boat_new() {
    Boat *new_boat = malloc(sizeof(Boat));

    new_boat->x = 0;
    new_boat->y = 0;
    new_boat->angle = 0;
    new_boat->sail_angle = 0;
    new_boat->rudder_angle = 0;

    load_boat_images(new_boat);

    return new_boat;
}

void sail_boat_free(Boat *boat) {
    g_object_unref(boat->images->hull);
    free(boat->images);
    free(boat);
}
