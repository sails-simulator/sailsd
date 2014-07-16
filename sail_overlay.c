#include <math.h>

#include <cairo.h>

#include "sail_boat.h"
#include "sail_overlay.h"
#include "sail_viewstate.h"

#define SAILS_OVERLAY_IS_BOTTOM FALSE

#define SAILS_OVERLAY_HEIGHT 64
#define SAILS_OVERLAY_PADDING 5

void draw_wind(cairo_t *cr) {
    cairo_arc(cr, SAILS_OVERLAY_HEIGHT / 2,
                  SAILS_OVERLAY_HEIGHT / 2,
                  (SAILS_OVERLAY_HEIGHT / 2) - SAILS_OVERLAY_PADDING,
                  0,
                  2 * M_PI);
    cairo_fill(cr);
}

void sail_overlay_draw(cairo_t *cr, Boat *boat, ViewState *view) {
    cairo_save(cr);
    cairo_identity_matrix(cr);

    if (SAILS_OVERLAY_IS_BOTTOM) {
        cairo_translate(cr, 0, view->height - SAILS_OVERLAY_HEIGHT);
    }
    cairo_rectangle(cr, 0, 0, view->width, SAILS_OVERLAY_HEIGHT);
    cairo_set_source_rgba(cr, 0, 0, 0, 0.8);
    cairo_fill(cr);

    draw_wind(cr);

    cairo_restore(cr);
}
