#ifndef SAIL_VIEWSTATE_H_
#define SAIL_VIEWSTATE_H_

struct _sim;

typedef struct _sim {
    double translation_x;
    double translation_y;
    double scale;
    gint width;
    gint height;

    // keys held down
    gboolean ctrl_held;
    gboolean is_fullscreen;
    gboolean tracking_boat;
    gboolean simulator_running;
    gboolean button_middle_held;
} ViewState;

ViewState* sail_viewstate_new();
void sail_viewstate_free(ViewState *view);

#endif
