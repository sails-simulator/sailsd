#include <stdio.h>
#include <cairo.h>
#include <glib.h>

#include "sim_view.h"

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
    for (n = -GRID_N; n < GRID_N; n++) {
        draw_x_gridline(cr, n);
        draw_y_gridline(cr, n);
    }
}

static void draw_axis(cairo_t *cr) {
    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 15);

    char number_text[GRID_NUMBER_STRING_LEN];
    int n;
    for (n = -GRID_WIDTH; n < GRID_WIDTH; n+=GRID_SPACING) {
        cairo_move_to(cr, n + 10, -10);
        snprintf(number_text, GRID_NUMBER_STRING_LEN, "%d", n/GRID_SPACING);
        cairo_show_text(cr, number_text);
    }
}

void sim_view_do_draw(cairo_t *cr, ViewState* sim) {
    cairo_translate(cr, (sim->width / 2) + sim->translation_x * sim->scale,
                        (sim->hight / 2) + sim->translation_y * sim->scale);

    cairo_scale(cr, sim->scale, sim->scale);
    cairo_set_source_rgb(cr, 0.7, 0.7, 1);
    cairo_paint(cr);

    draw_grid(cr);
    draw_axis(cr);
}
