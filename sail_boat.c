#include <stdlib.h>
#include <math.h>

#include <glib.h>
#include <cairo.h>
#include <librsvg/rsvg.h>

#include "sail_boat.h"
#include "sail_view.h"

static RsvgHandle* load_svg(const char *path) {
    GError *err = NULL;
    RsvgHandle *image = rsvg_handle_new_from_file(path, &err);
    if (err != NULL) {
        g_log("sails", G_LOG_LEVEL_ERROR, "can't open file \"%s\": %s", path, err->message);
    } else {
        g_message("loaded image \"%s\"", path);
    }
    return image;
}

static Boat* load_boat_images(Boat *boat) {
    boat->images = malloc(sizeof(SVGImages));

    #if !GLIB_CHECK_VERSION(2, 35, 0)
        g_type_init();
    #endif

    boat->images->hull = load_svg("resources/hull.svg");
    boat->images->hull_dimensions = malloc(sizeof(RsvgDimensionData));
    rsvg_handle_get_dimensions(boat->images->hull,
                               boat->images->hull_dimensions);

    boat->images->sail = load_svg("resources/sail.svg");
    boat->images->sail_dimensions = malloc(sizeof(RsvgDimensionData));
    rsvg_handle_get_dimensions(boat->images->sail,
                               boat->images->sail_dimensions);

    boat->images->rudder = load_svg("resources/rudder.svg");
    boat->images->rudder_dimensions = malloc(sizeof(RsvgDimensionData));
    rsvg_handle_get_dimensions(boat->images->rudder,
                               boat->images->rudder_dimensions);

    return boat;
}

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
    new_boat->angle = 0;
    new_boat->sail_angle = 0;
    new_boat->rudder_angle = 0.2;
    new_boat->sail_is_free = 0;

    new_boat->theta=M_PI_4;
    new_boat->v=5;
    new_boat->sheet_length = 0;
    new_boat->rotational_velocity = 0.0;
    new_boat->mass = 10000.0;
    new_boat->drift_coefficient = 0.05;
    new_boat->rudder_distance = 4.0;
    new_boat->angular_friction = 6000;
    new_boat->m=300.0;
    new_boat->tangential_friction = 0.2;
    new_boat->mast_distance = 1;
    new_boat->rudder_lift = 2000.0;
    new_boat->sail_center_of_effort = 1.0;
    new_boat->sail_lift=1000.0;

    load_boat_images(new_boat);

    return new_boat;
}

void sail_boat_free(Boat *boat) {
    g_object_unref(boat->images->hull);
    g_object_unref(boat->images->sail);
    free(boat->images);
    free(boat);
}
