#include <math.h>

#include "sail_boat.h"
#include "sail_physics.h"

double sign(double a) {
    if (a <= 0) {
        return -1;
    } else {
        return 1;
    }
}

void sail_physics_update(Boat *boat, const double dt) {
    boat->angle = boat->angle + 0.1 * dt;
    double dell = 0;
    double deltag = boat->rudder_angle;
    boat->ell=boat->ell+dt*dell;

    double xw_ap = boat->a * cos(boat->psi - boat->theta) - boat->v;
    double yw_ap = boat->a * sin(boat->psi - boat->theta);
    boat->psi_ap = atan2(yw_ap, xw_ap);
    boat->a_ap = sqrt(xw_ap*xw_ap + yw_ap*yw_ap);
    boat->gamma = cos(boat->psi_ap) + cos(boat->ell);


    if (boat->gamma < 0) {
        boat->deltav = atan(tan(boat->psi_ap));
        boat->ell=fabs(boat->deltav);
    } else {
        boat->deltav = sign(sin(-boat->psi_ap))*boat->ell;
    }

    boat->fg = boat->alphag * boat->v * sin(deltag);
    boat->fv = boat->alphav * boat->a_ap * sin(boat->deltav - boat->psi_ap);
    boat->x += (boat->v * cos(boat->theta) + boat->beta * boat->a * cos(boat->psi)) * dt;
    boat->y += (boat->v * sin(boat->theta) + boat->beta * boat->a * sin(boat->psi)) * dt;

    boat->omega += (1/boat->Jz) *
                   ((boat->l - boat->rv * cos(boat->deltav)) *
                    boat->fv - boat->rg * cos(deltag) * boat->fg - boat->alphatheta *
                    boat->omega * boat->v) * dt;
    boat->v += (1/boat->m) * (sin(boat->deltav) * boat->fv - sin(deltag) *
               boat->fg - boat->alphaf * boat->v * boat->v) * dt;

    boat->theta += boat->omega * dt;
    boat->angle = boat->theta;
} 
