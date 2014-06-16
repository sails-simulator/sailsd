#include <stdio.h>
#include <cairo.h>
#include <gtk/gtk.h>

#define GRID_SPACING 100
#define GRID_WIDTH 10000
#define GRID_N GRID_WIDTH / GRID_SPACING

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

static void do_draw(cairo_t *cr) {
    cairo_set_source_rgb(cr, 0.7, 0.7, 1);
    cairo_paint(cr);
    cairo_set_line_width(cr, 1);

    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    int n;
    for (n = 0; n < GRID_N; n++) {
        draw_x_gridline(cr, n);
        draw_y_gridline(cr, n);
    }

    cairo_move_to(cr, 240, 40);
    cairo_line_to(cr, 240, 160);
    cairo_line_to(cr, 350, 160);
    cairo_close_path(cr);

    cairo_stroke_preserve(cr);
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    do_draw(cr);
    return FALSE;
}

static gboolean on_scroll_event(GtkWidget *widget, GdkEvent *ev) {
    puts("scrolled");
    return FALSE;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *draw;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    draw = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), draw);

    g_signal_connect(G_OBJECT(draw), "draw",
                     G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "scroll-event",
                     G_CALLBACK(on_scroll_event), NULL);
    gtk_widget_add_events(window, GDK_SCROLL_MASK | GDK_KEY_PRESS_MASK);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 854, 480);
    gtk_window_set_title(GTK_WINDOW(window), "sailsim");

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
