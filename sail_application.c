#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <gtk/gtk.h>

#include "sail_view.h"
#include "sail_viewstate.h"
#include "sail_boat.h"

#define SAIL_MIN_WIDTH 640
#define SAIL_MIN_HEIGHT 360
#define SAIL_DEFAULT_WIDTH 854
#define SAIL_DEFAULT_HEIGHT 480

typedef struct _sail_states {
    Boat *boat;
    ViewState *view;
} SailState;

static SailState* sail_state_new() {
    SailState *states = malloc(sizeof(SailState));
    states->view = sail_viewstate_new();
    states->boat = sail_boat_new();
    return states;
}

static void sail_state_free(SailState *state) {
    sail_boat_free(state->boat);
    sail_viewstate_free(state->view);
    free(state);
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, SailState *state) {
    sail_view_do_draw(cr,
                     state->view->width, state->view->hight,
                     state->view->translation_x, state->view->translation_y,
                     state->view->scale);
    sail_boat_draw(state->boat, cr);
    return FALSE;
}

static gboolean on_scroll_event(GtkWidget *widget, GdkEvent *ev, SailState *state) {
    GdkScrollDirection scroll = 0;
    ViewState *sim = state->view;

    if (gdk_event_get_scroll_deltas(ev, 0, 0) == FALSE) {
        gdk_event_get_scroll_direction(ev, &scroll);

        if (sim->ctrl_held) {
            if (scroll == GDK_SCROLL_UP) {
                sim->scale += sim->scale * 0.05;
            } else if (scroll == GDK_SCROLL_DOWN) {
                sim->scale -= sim->scale * 0.05;
            }
        } else {

            double scroll_distance = 3 / sim->scale;
            switch (scroll) {
                case GDK_SCROLL_UP:
                    sim->translation_y += scroll_distance;
                    break;
                case GDK_SCROLL_DOWN:
                    sim->translation_y -= scroll_distance;
                    break;
                case GDK_SCROLL_LEFT:
                    sim->translation_x += scroll_distance;
                    break;
                case GDK_SCROLL_RIGHT:
                    sim->translation_x -= scroll_distance;
                    break;
                case GDK_SCROLL_SMOOTH:
                    break;
            }
        }
        gtk_widget_queue_draw(widget);
    }
    return FALSE;
}

static void on_quit(SailState *state) {
    g_message("Qutting...");
    sail_state_free(state);
    gtk_main_quit();
}

static gboolean on_key_press_event(GtkWidget *widget, GdkEvent *ev, SailState *state) {
    guint val = 0;
    gdk_event_get_keyval(ev, &val);
    if (val == GDK_KEY_Escape) {
        on_quit(state);
    } else if (val == GDK_KEY_Control_L || val == GDK_KEY_Control_R) {
        state->view->ctrl_held = TRUE;
    }
    return FALSE;
}

static gboolean on_key_release_event(GtkWidget *widget, GdkEvent *ev, SailState *state) {
    guint val = 0;
    gdk_event_get_keyval(ev, &val);
    if (val == GDK_KEY_Control_L || val == GDK_KEY_Control_R) {
        state->view->ctrl_held = FALSE;
    }
    return FALSE;
}

static gboolean on_configure_event(GtkWidget *widget, GdkEvent *ev, SailState *state) {
    gtk_window_get_size(GTK_WINDOW(widget), &state->view->width, &state->view->hight);
    return FALSE;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *draw;
    GdkGeometry hints;
    hints.min_width = SAIL_MIN_WIDTH;
    hints.min_height = SAIL_MIN_HEIGHT;

    SailState *states = sail_state_new();

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    draw = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), draw);

    g_signal_connect(G_OBJECT(draw), "draw",
                     G_CALLBACK(on_draw_event), states);
    g_signal_connect(window, "destroy",
                     G_CALLBACK(on_quit), states);
    g_signal_connect(window, "scroll-event",
                     G_CALLBACK(on_scroll_event), states);
    g_signal_connect(window, "key-press-event",
                     G_CALLBACK(on_key_press_event), states);
    g_signal_connect(window, "key-release-event",
                     G_CALLBACK(on_key_release_event), states);
    g_signal_connect(window, "configure-event",
                     G_CALLBACK(on_configure_event), states);

    gtk_widget_add_events(window, GDK_SCROLL_MASK | GDK_KEY_PRESS_MASK);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window),
                                SAIL_DEFAULT_WIDTH, SAIL_DEFAULT_HEIGHT);
    gtk_window_set_geometry_hints(GTK_WINDOW(window), NULL,
                                  &hints, GDK_HINT_MIN_SIZE);
    gtk_window_set_title(GTK_WINDOW(window), "sailsim");

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
