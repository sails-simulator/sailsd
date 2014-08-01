#include <stdlib.h>
#include <math.h>

#include "sail_wind.h"

Wind* sail_wind_new() {
    Wind *new_wind = malloc(sizeof(Wind));

    new_wind->a=4;
    new_wind->psi=M_PI_2;

    return new_wind;
}
