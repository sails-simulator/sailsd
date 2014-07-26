#ifndef SAIL_VIEW_H_
#define SAIL_VIEW_H_

#define SAILS_GRID_SPACING 100
#define SAILS_GRID_WIDTH 10000
#define SAILS_GRID_N SAILS_GRID_WIDTH / SAILS_GRID_SPACING

#define SAILS_GRID_NUMBER_STRING_LEN 8

void sail_view_draw(cairo_t *cr,
                    int width, int hight,
                    double tx, double ty,
                    double scale);

#endif
