typedef struct _wind {
    double speed, direction, psi_ap, a_ap;
} Wind;

Wind* sail_wind_new();

double sail_wind_get_speed(const Wind *wind);
double sail_wind_get_direction(const Wind *wind);
