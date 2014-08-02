#include <math.h>

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
    return sail_wind_get_speed(wind) * cos(sail_wind_get_direction(wind) - boat->theta) - boat->v;
}

static double apparent_wind_y(const Boat *boat, const Wind *wind) {
    return sail_wind_get_speed(wind) * sin(sail_wind_get_direction(wind) - boat->theta);
}

static double apparent_wind_direction(const Boat *boat, const Wind *wind) {
    return atan2(apparent_wind_y(boat, wind), apparent_wind_x(boat, wind));
}

static double apparent_wind_speed(const Boat *boat, const Wind *wind) {
    return sqrt(pow(apparent_wind_x(boat, wind), 2) + 
                pow(apparent_wind_y(boat, wind), 2));
}

static gboolean mainsheet_is_tight(const Boat *boat, const Wind *wind) {
    if (cos(apparent_wind_direction(boat, wind)) + cos(boat->ell) < 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static double force_on_rudder(const Boat *boat, const Wind *wind) {
    return boat->rudder_lift * boat->v * sin(sail_boat_get_rudder_angle(boat));
}

static double force_on_sail(const Boat *boat, const Wind *wind) {
    return boat->sail_lift * apparent_wind_speed(boat, wind) * sin(boat->deltav - apparent_wind_direction(boat, wind));
}

static gboolean sail_is_in_bounds(Boat *boat) {
    if (boat->ell > -M_PI_2 && boat->ell < M_PI_2) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void sail_physics_update(Boat *boat, const Wind *wind, const double dt) {
    double deltag = boat->rudder_angle;

    if (sail_is_in_bounds(boat)) {
        boat->ell = boat->ell + dt * boat->sail_is_free;
    }

    if (mainsheet_is_tight(boat, wind)) {
        boat->deltav = atan(tan(apparent_wind_direction(boat, wind)));

        // make sure the sail can change side
        if (!fabs(boat->deltav)) {
            boat->ell = fabs(boat->deltav);
        }
    } else {
        boat->deltav = sign_of(sin(-apparent_wind_direction(boat, wind)))*boat->ell;
    }

    boat->x += (boat->v * cos(boat->theta) + boat->beta * sail_wind_get_speed(wind) * cos(sail_wind_get_direction(wind))) * dt;
    boat->y += (boat->v * sin(boat->theta) + boat->beta * sail_wind_get_speed(wind) * sin(sail_wind_get_direction(wind))) * dt;

    boat->omega += (1/boat->Jz) *
                   ((boat->l - boat->rv * cos(boat->deltav)) *
                    force_on_sail(boat, wind) - boat->rg * cos(deltag) * force_on_rudder(boat, wind) - boat->alphatheta *
                    boat->omega * boat->v) * dt;
    boat->v += (1/boat->m) * (sin(boat->deltav) * force_on_sail(boat, wind) - sin(deltag) *
               force_on_rudder(boat, wind) - boat->alphaf * boat->v * boat->v) * dt;

    boat->theta += boat->omega * dt;
    boat->angle = boat->theta;
}
