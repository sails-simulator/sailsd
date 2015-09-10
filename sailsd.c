#include <stdlib.h>

#include <gio/gio.h>
#include <glib.h>

#include "sail_boat.h"
#include "sail_physics.h"
#include "sail_wind.h"


static GDBusNodeInfo *introspection_data = NULL;

static Boat *boat;
static Wind *wind;

static const gchar introspection_xml[] =
    "<node>"
    "  <interface name='eu.kragniz.sails.Boat'>"
    "    <property name='Bearing' type='d' access='read'/>"
    "    <property name='Location' type='(dd)' access='read'/>"
    "  </interface>"
    "  <interface name='eu.kragniz.sails.Wind'>"
    "    <property name='Direction' type='d' access='readwrite'/>"
    "    <property name='Speed' type='d' access='readwrite'/>"
    "  </interface>"
    "</node>";

static GMainLoop *loop;

static gpointer physics_thread(gpointer data)
{
    for (;;) {
        sail_physics_update(boat, wind, 0.000001);
        g_print("Boat x: %f, y: %f\n", boat->x, boat->y);
        g_usleep(100000);
    }

    return NULL;
}

static GVariant *handle_get_property(GDBusConnection  *connection,
                                     const gchar      *sender,
                                     const gchar      *object_path,
                                     const gchar      *interface_name,
                                     const gchar      *property_name,
                                     GError          **error,
                                     gpointer          user_data)
{
    GVariant *ret = NULL;

    if (g_strcmp0(property_name, "Bearing") == 0) {
        ret = g_variant_new_double(sail_boat_get_angle(boat));
    }

    else if (g_strcmp0(property_name, "Location") == 0) {
        ret = g_variant_new("(dd)", boat->x, boat->y);
    }

    else if (g_strcmp0(property_name, "Direction") == 0) {
        ret = g_variant_new_double(sail_wind_get_direction(wind));
    }

    else if (g_strcmp0(property_name, "Speed") == 0) {
        ret = g_variant_new_double(sail_wind_get_speed(wind));
    }

    return ret;
}

static gboolean handle_set_property(GDBusConnection  *connection,
                                    const gchar      *sender,
                                    const gchar      *object_path,
                                    const gchar      *interface_name,
                                    const gchar      *property_name,
                                    GVariant         *value,
                                    GError          **error,
                                    gpointer          user_data)
{

    if (g_strcmp0(property_name, "Direction") == 0) {
        wind->direction = g_variant_get_double(value);
    }

    return NULL;
}

static const GDBusInterfaceVTable interface_vtable =
{
    NULL,
    handle_get_property,
    handle_set_property
};

static void on_bus_acquired(GDBusConnection *connection,
                            const gchar     *name,
                            gpointer         user_data)
{
    guint registration_id;
    registration_id = g_dbus_connection_register_object(connection,
                                                        "/sails/boats/Boat1",
                                                        introspection_data->interfaces[0],
                                                        &interface_vtable,
                                                        NULL,
                                                        NULL,
                                                        NULL);
    g_assert(registration_id > 0);

    registration_id = g_dbus_connection_register_object(connection,
                                                        "/sails/Wind",
                                                        introspection_data->interfaces[1],
                                                        &interface_vtable,
                                                        NULL,
                                                        NULL,
                                                        NULL);
    g_assert(registration_id > 0);
}

static void on_name_acquired(GDBusConnection *connection,
                             const gchar     *name,
                             gpointer         user_data)
{
    // start the physics model thread after we're connected to the bus and acquired name
    g_thread_new("physics_thread", physics_thread, NULL);
}

static void on_name_lost(GDBusConnection *connection,
                         const gchar     *name,
                         gpointer         user_data)
{
    exit(1);
}

int main(int argc, char *argv[])
{
    boat = sail_boat_new();
    wind = sail_wind_new();

    loop = g_main_loop_new(NULL, FALSE);
    guint owner_id;

    introspection_data = g_dbus_node_info_new_for_xml(introspection_xml, NULL);
    g_assert(introspection_data != NULL);

    owner_id = g_bus_own_name(G_BUS_TYPE_SESSION,
                              "eu.kragniz.sails",
                              G_BUS_NAME_OWNER_FLAGS_NONE,
                              on_bus_acquired,
                              on_name_acquired,
                              on_name_lost,
                              NULL,
                              NULL);

    g_print("Entering main loop\n");
    g_main_loop_run(loop);

    g_bus_unown_name(owner_id);
    g_dbus_node_info_unref(introspection_data);

    return 0;
}
