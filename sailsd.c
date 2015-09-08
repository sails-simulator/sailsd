#include <stdio.h>
#include <stdlib.h>
#include <gio/gio.h>

static GDBusNodeInfo *introspection_data = NULL;

static const gchar introspection_xml[] =
    "<node>"
    "  <interface name='eu.kragniz.sails.Boat'>"
    "    <property name='Bearing' type='d' access='read'/>"
    "    <property name='Location' type='(dd)' access='read'/>"
    "  </interface>"
    "</node>";

static GMainLoop *loop;

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
        ret = g_variant_new_double(42.2);
    }

    else if (g_strcmp0(property_name, "Location") == 0) {
        ret = g_variant_new("(ii)", 1, 2);
    }

    return ret;
}

static const GDBusInterfaceVTable interface_vtable =
{
    NULL,
    handle_get_property,
    NULL
};

static void on_bus_acquired(GDBusConnection *connection,
                            const gchar     *name,
                            gpointer         user_data)
{
    guint registration_id;
    registration_id = g_dbus_connection_register_object(connection,
                                                        "/eu/kragniz/sails/Boat1",
                                                        introspection_data->interfaces[0],
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
    /* something */
}

static void on_name_lost(GDBusConnection *connection,
                         const gchar     *name,
                         gpointer         user_data)
{
    exit(1);
}

int main(int argc, char *argv[])
{
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

    g_print("Entering main loop");
    g_main_loop_run(loop);

    g_bus_unown_name(owner_id);
    g_dbus_node_info_unref(introspection_data);
}
