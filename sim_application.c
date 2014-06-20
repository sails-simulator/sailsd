#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <gtk/gtk.h>

#include "sim_view.h"
#include "sim_viewstate.h"

#define SIM_MIN_WIDTH 640
#define SIM_MIN_HEIGHT 360
#define SIM_DEFAULT_WIDTH 854
#define SIM_DEFAULT_HEIGHT 480

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, ViewState* sim) {
    sim_view_do_draw(cr, sim);
    return FALSE;
}

static gboolean on_scroll_event(GtkWidget *widget, GdkEvent *ev, ViewState* sim) {
    GdkScrollDirection scroll = 0;

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

static void on_quit() {
    g_message("Qutting...");
    gtk_main_quit();
}

static gboolean on_key_press_event(GtkWidget *widget, GdkEvent *ev, ViewState* sim) {
    guint val = 0;
    gdk_event_get_keyval(ev, &val);
    if (val == GDK_KEY_Escape) {
        on_quit();
    } else if (val == GDK_KEY_Control_L || val == GDK_KEY_Control_R) {
        sim->ctrl_held = TRUE;
    }
    return FALSE;
}

static gboolean on_key_release_event(GtkWidget *widget, GdkEvent *ev, ViewState* sim) {
    guint val = 0;
    gdk_event_get_keyval(ev, &val);
    if (val == GDK_KEY_Control_L || val == GDK_KEY_Control_R) {
        sim->ctrl_held = FALSE;
    }
    return FALSE;
}

static gboolean on_configure_event(GtkWidget *widget, GdkEvent *ev, ViewState* sim) {
    gtk_window_get_size(GTK_WINDOW(widget), &sim->width, &sim->hight);
    return FALSE;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *draw;
    GdkGeometry hints;
    hints.min_width = SIM_MIN_WIDTH;
    hints.min_height = SIM_MIN_HEIGHT;

    ViewState* sim = viewstate_new();

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    draw = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), draw);

    g_signal_connect(G_OBJECT(draw), "draw",
                     G_CALLBACK(on_draw_event), sim);
    g_signal_connect(window, "destroy",
                     G_CALLBACK(on_quit), NULL);
    g_signal_connect(window, "scroll-event",
                     G_CALLBACK(on_scroll_event), sim);
    g_signal_connect(window, "key-press-event",
                     G_CALLBACK(on_key_press_event), sim);
    g_signal_connect(window, "key-release-event",
                     G_CALLBACK(on_key_release_event), sim);
    g_signal_connect(window, "configure-event",
                     G_CALLBACK(on_configure_event), sim);

    gtk_widget_add_events(window, GDK_SCROLL_MASK | GDK_KEY_PRESS_MASK);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window),
                                SIM_DEFAULT_WIDTH, SIM_DEFAULT_HEIGHT);
    gtk_window_set_geometry_hints(GTK_WINDOW(window), NULL,
                                  &hints, GDK_HINT_MIN_SIZE);
    gtk_window_set_title(GTK_WINDOW(window), "sailsim");

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
