#include <math.h>

#include <glib.h>

#include "sail_boat.h"
#include "sail_physics.h"
#include "sail_wind.h"

static double sign_of(double a) {
    if (a <= 0) {
        return -1;
    } else {
        return 1;
    }
}

static double apparent_wind_x(const Boat *boat, const Wind *wind) {
    return sail_wind_get_speed(wind) * cos(sail_wind_get_direction(wind) - boat->angle) - boat->v;
}

static double apparent_wind_y(const Boat *boat, const Wind *wind) {
    return sail_wind_get_speed(wind) * sin(sail_wind_get_direction(wind) - boat->angle);
}

static double apparent_wind_direction(const Boat *boat, const Wind *wind) {
    return atan2(apparent_wind_y(boat, wind), apparent_wind_x(boat, wind));
}

static double apparent_wind_speed(const Boat *boat, const Wind *wind) {
    return sqrt(pow(apparent_wind_x(boat, wind), 2) + 
                pow(apparent_wind_y(boat, wind), 2));
}

static gboolean mainsheet_is_tight(const Boat *boat, const Wind *wind) {
    if (cos(apparent_wind_direction(boat, wind)) + cos(boat->sheet_length) < 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static double force_on_rudder(const Boat *boat, const Wind *wind) {
    return boat->rudder_lift * boat->v * sin(sail_boat_get_rudder_angle(boat));
}

static double force_on_sail(const Boat *boat, const Wind *wind) {
    return boat->sail_lift * apparent_wind_speed(boat, wind) * sin(boat->sail_angle - apparent_wind_direction(boat, wind));
}

static gboolean sail_is_in_bounds(const Boat *boat) {
    if (boat->sheet_length > -M_PI_2 && boat->sheet_length < M_PI_2) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static double delta_x(const Boat *boat, const Wind *wind) {
    return sail_boat_get_velocity(boat) * cos(sail_boat_get_angle(boat)) +
           boat->drift_coefficient *
           sail_wind_get_speed(wind) *
           cos(sail_wind_get_direction(wind));
}

static double delta_y(const Boat *boat, const Wind *wind) {
    return sail_boat_get_velocity(boat) * sin(sail_boat_get_angle(boat)) +
           boat->drift_coefficient *
           sail_wind_get_speed(wind) *
           sin(sail_wind_get_direction(wind));
}

static double delta_rotational_velocity(const Boat *boat, const Wind *wind) {
    return ((boat->sail_center_of_effort - boat->mast_distance * cos(boat->sail_angle)) * force_on_sail(boat, wind) -
            boat->rudder_distance * cos(sail_boat_get_rudder_angle(boat)) * force_on_rudder(boat, wind) -
            boat->angular_friction * boat->rotational_velocity * boat->v) / boat->inertia;
}

static double delta_velocity(const Boat *boat, const Wind *wind) {
    return (sin(boat->sail_angle) * force_on_sail(boat, wind) -
            sin(sail_boat_get_rudder_angle(boat)) * force_on_rudder(boat, wind) -
            boat->tangential_friction * boat->v * boat->v) / boat->mass;
}

void sail_physics_update(Boat *boat, const Wind *wind, const double dt) {
    if (sail_is_in_bounds(boat)) {
        boat->sheet_length = boat->sheet_length + dt * boat->sail_is_free;
    }

    if (mainsheet_is_tight(boat, wind)) {
        boat->sail_angle = atan(tan(apparent_wind_direction(boat, wind)));

        // make sure the sail can change side
        if (!fabs(boat->sail_angle)) {
            boat->sheet_length = fabs(boat->sail_angle);
        }
    } else {
        boat->sail_angle = sign_of(sin(-apparent_wind_direction(boat, wind)))*boat->sheet_length;
    }

    boat->x += delta_x(boat, wind) * dt;
    boat->y += delta_y(boat, wind) * dt;

    boat->rotational_velocity += delta_rotational_velocity(boat, wind) * dt;
    boat->v += delta_velocity(boat, wind) * dt;
    boat->angle += boat->rotational_velocity * dt;
}
