#include "gps.h"

double gps_degToRad(double deg)
{
    return (deg * GPS_PI / 180);
}

double gps_radToDeg(double rad)
{
    return (rad * 180 / GPS_PI);
}

double gps_distanceLL(double lat1, double lon1, double lat2, double lon2)
{
    lat1 = gps_degToRad(lat1);
    lon1 = gps_degToRad(lon1);
    lat2 = gps_degToRad(lat2);
    lon2 = gps_degToRad(lon2);
    double u = sin((lat2 - lat1) / 2);
    double v = sin((lon2 - lon1) / 2);
    return 2.0 * GPS_EARTH_RADIUS_KM * asin(sqrt(u * u + cos(lat1) * cos(lat2) * v * v));
}

bool gps_isValidLLStr(char *str)
{
    double d1, d2;
    if (sscanf(str, "%lf,%lf", &d1, &d2) != 2)
        return false;
    return true;
}

bool gps_isValidDouble(char *str)
{
    double d;
    if (sscanf(str, "%lf", &d) != 1)
        return false;
    return true;
}