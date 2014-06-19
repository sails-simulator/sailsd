#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <gtk/gtk.h>

#define GRID_SPACING 100
#define GRID_WIDTH 10000
#define GRID_N GRID_WIDTH / GRID_SPACING

typedef struct _sim {
    double translation_x;
    double translation_y;
    double scale;
    gint width;
    gint hight;

    // keys held down
    gboolean ctrl_held;
} ViewState;

static ViewState* viewstate_new() {
    ViewState *new_state = malloc(sizeof(ViewState));
    new_state->translation_x = 0;
    new_state->translation_y = 0;
    new_state->scale = 1;

    new_state->width = 1;
    new_state->hight = 1;

    new_state->ctrl_held = FALSE;
    return new_state;
}

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

static void do_draw(cairo_t *cr, ViewState* sim) {
    cairo_translate(cr, (sim->width / 2) + sim->translation_x * sim->scale,
                        (sim->hight / 2) + sim->translation_y * sim->scale);

    cairo_scale(cr, sim->scale, sim->scale);
    cairo_set_source_rgb(cr, 0.7, 0.7, 1);
    cairo_paint(cr);

    draw_grid(cr);
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, ViewState* sim) {
    do_draw(cr, sim);
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

static void sim_quit() {
    g_message("Qutting...");
    gtk_main_quit();
}

static gboolean on_key_press_event(GtkWidget *widget, GdkEvent *ev, ViewState* sim) {
    guint val = 0;
    gdk_event_get_keyval(ev, &val);
    if (val == GDK_KEY_Escape) {
        sim_quit();
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

    ViewState* sim = viewstate_new();

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    draw = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), draw);

    g_signal_connect(G_OBJECT(draw), "draw",
                     G_CALLBACK(on_draw_event), sim);
    g_signal_connect(window, "destroy",
                     G_CALLBACK(sim_quit), NULL);
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
    gtk_window_set_default_size(GTK_WINDOW(window), 854, 480);
    gtk_window_set_title(GTK_WINDOW(window), "sailsim");

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
