#include <cairo.h>

#include "sail_boat.h"
#include "sail_overlay.h"
#include "sail_viewstate.h"

void sail_overlay_draw(cairo_t *cr, Boat *boat, ViewState *view) {
    cairo_save(cr);
    cairo_identity_matrix(cr);

    cairo_rectangle(cr, 0, 0, view->width, 50);
    cairo_set_source_rgba(cr, 0, 0, 0, 0.8);
    cairo_fill(cr);

    cairo_restore(cr);
}
