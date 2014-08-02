#ifndef SAIL_WIND_H_
#define SAIL_WIND_H_

typedef struct _wind {
    double speed, direction;
} Wind;

Wind* sail_wind_new();

double sail_wind_get_speed(const Wind *wind);
double sail_wind_get_direction(const Wind *wind);

#endif
