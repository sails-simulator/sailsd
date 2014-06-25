#include <stdio.h>
#include <cairo.h>
#include <glib.h>

#include "sail_view.h"

#define GRID_SPACING 100
#define GRID_WIDTH 10000
#define GRID_N GRID_WIDTH / GRID_SPACING

#define GRID_NUMBER_STRING_LEN 8

static void draw_x_gridline(cairo_t *cr, int n) {
    int x = n * GRID_SPACING;
    cairo_move_to(cr, x + 0.5, -GRID_WIDTH);
    cairo_line_to(cr, x + 0.5, GRID_WIDTH);
    cairo_stroke(cr);
}

static void draw_y_gridline(cairo_t *cr, int n) {
    int y = n * GRID_SPACING;
    cairo_move_to(cr, -GRID_WIDTH, y + 0.5);
    cairo_line_to(cr, GRID_WIDTH, y + 0.5);
    cairo_stroke(cr);
}

static void draw_grid(cairo_t *cr) {
    cairo_set_line_width(cr, 1);

    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    int n;
    for (n = -GRID_N; n <= GRID_N; n++) {
        draw_x_gridline(cr, n);
        draw_y_gridline(cr, n);
    }
}

static void draw_axis(cairo_t *cr) {
    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 11);

    char number_text[GRID_NUMBER_STRING_LEN];
    int n;
    for (n = -GRID_WIDTH; n <= GRID_WIDTH; n += GRID_SPACING) {
        cairo_move_to(cr, n + 10, -10);
        snprintf(number_text, GRID_NUMBER_STRING_LEN, "%d", n/GRID_SPACING);
        cairo_show_text(cr, number_text);
    }
}

void sail_view_do_draw(cairo_t *cr,
                      int width, int hight,
                      double tx, double ty,
                      double scale) {
    cairo_translate(cr, (width / 2) + tx * scale,
                        (hight / 2) + ty * scale);

    cairo_scale(cr, scale, scale);
    cairo_set_source_rgb(cr, 0.7, 0.7, 1);
    cairo_paint(cr);

    draw_grid(cr);
    draw_axis(cr);
}
