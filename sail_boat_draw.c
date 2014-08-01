#include <math.h>

#include <librsvg/rsvg.h>

#ifndef LIBRSVG_CHECK_VERSION
    #include <librsvg/rsvg-cairo.h>
#endif

#include "sail_boat_draw.h"
#include "sail_view.h"

static void draw_hull(const Boat *boat, cairo_t *cr) {
    cairo_save(cr);
    cairo_translate(cr, -boat->images->hull_dimensions->width/2,
                        -boat->images->hull_dimensions->height/2);
    rsvg_handle_render_cairo(boat->images->hull, cr);
    cairo_restore(cr);
}

static void draw_sail(const Boat *boat, cairo_t *cr) {
    cairo_save(cr);

    cairo_translate(cr, 0, -boat->images->hull_dimensions->height/5);
    cairo_rotate(cr, -boat->deltav);
    cairo_translate(cr, -boat->images->sail_dimensions->width/2, -boat->images->sail_dimensions->height/10);

    rsvg_handle_render_cairo(boat->images->sail, cr);

    cairo_restore(cr);
}

static void draw_rudder(const Boat *boat, cairo_t *cr) {
    cairo_save(cr);

    cairo_translate(cr, 0, boat->images->hull_dimensions->height/2);
    cairo_rotate(cr, -sail_boat_get_rudder_angle(boat));
    cairo_translate(cr, -boat->images->rudder_dimensions->width/2,
                        -boat->images->rudder_dimensions->height/10);

    rsvg_handle_render_cairo(boat->images->rudder, cr);

    cairo_restore(cr);
}

void sail_boat_draw(cairo_t *cr, const Boat *boat) {
    cairo_save(cr);
    cairo_translate(cr, boat->x * SAILS_GRID_SPACING,
                        -boat->y * SAILS_GRID_SPACING);
    cairo_rotate(cr, -boat->angle + M_PI_2);

    draw_hull(boat, cr);
    draw_rudder(boat, cr);
    draw_sail(boat, cr);

    cairo_restore(cr);
}
