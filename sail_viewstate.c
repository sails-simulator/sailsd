#include <stdlib.h>
#include <glib.h>

#include "sail_viewstate.h"

ViewState* sail_viewstate_new() {
    ViewState *new_state = malloc(sizeof(ViewState));
    new_state->translation_x = 0;
    new_state->translation_y = 0;
    new_state->scale = 1;

    new_state->width = 1;
    new_state->hight = 1;

    new_state->ctrl_held = FALSE;
    return new_state;
}

void sail_viewstate_free(ViewState *view){
    free(view);
}
