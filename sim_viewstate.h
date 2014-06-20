#ifndef SIM_VIEWSTATE_H_
#define SIM_VIEWSTATE_H_

struct _sim;

typedef struct _sim {
    double translation_x;
    double translation_y;
    double scale;
    gint width;
    gint hight;

    // keys held down
    gboolean ctrl_held;
} ViewState;

ViewState* viewstate_new();

#endif
