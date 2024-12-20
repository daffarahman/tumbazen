#ifndef _GPS_H_
#define _GPS_H_

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define GPS_EARTH_RADIUS_KM 6731.0
#define GPS_PI 3.14159265358979323846

typedef struct
{
    double lat;
    double lon;
} gps_LL;

double gps_degToRad(double deg);
double gps_radToDeg(double rad);
double gps_distanceLL(double lat1, double lon1, double lat2, double lon2);
bool gps_isValidLLStr(char *str);
bool gps_isValidDouble(char *str);

#endif