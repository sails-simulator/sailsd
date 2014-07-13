#ifndef SAIL_VIEW_H_
#define SAIL_VIEW_H_

#define SAIL_GRID_SPACING 100
#define SAIL_GRID_WIDTH 10000
#define SAIL_GRID_N SAIL_GRID_WIDTH / SAIL_GRID_SPACING

#define SAIL_GRID_NUMBER_STRING_LEN 8

void sail_view_draw(cairo_t *cr,
                    int width, int hight,
                    double tx, double ty,
                    double scale);

#endif
