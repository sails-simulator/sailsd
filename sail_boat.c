#include <stdlib.h>
#include <math.h>

#include <glib.h>
#include <cairo.h>
#include <librsvg/rsvg.h>

#include "sail_boat.h"

static RsvgHandle* load_svg(char *path) {
    GError *err = NULL;
    RsvgHandle *image = rsvg_handle_new_from_file(path, &err);
    if (err != NULL) {
        g_log("sail", G_LOG_LEVEL_ERROR, "can't open file \"%s\": %s", path, err->message);
    } else {
        g_log("sail", G_LOG_LEVEL_MESSAGE, "loaded image \"%s\"", path);
    }
    return image;
}

static Boat* load_boat_images(Boat *boat) {
    boat->images = malloc(sizeof(SVGImages));

    #if !GLIB_CHECK_VERSION(2,35,0)
        g_type_init();
    #endif

    boat->images->hull = load_svg("resources/hull.svg");
    boat->images->hull_dimensions = malloc(sizeof(RsvgDimensionData));
    rsvg_handle_get_dimensions(boat->images->hull,
                               boat->images->hull_dimensions);
    return boat;
}

void sail_boat_draw(Boat *boat, cairo_t *cr) {
    cairo_save(cr);
    cairo_rotate(cr, boat->angle);
    cairo_translate(cr, -boat->images->hull_dimensions->width/2,
                        -boat->images->hull_dimensions->height/2);
    rsvg_handle_render_cairo(boat->images->hull, cr);
    cairo_restore(cr);
}

Boat* sail_boat_new() {
    Boat *new_boat = malloc(sizeof(Boat));

    new_boat->x = 0;
    new_boat->y = 0;
    new_boat->angle = M_PI_4;
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
