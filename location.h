#ifndef LOCATION_H
#define LOCATION_H

//includes
#include <cstdlib> // labs
#include <cmath>
#include "global_defs.h"

//defines
#define radians(x)	 3.14159265*(x)/180
#define degrees(x)	 1/3.14159265*(x)*180

//types
struct Loc {
    long lat;                ///< param 3 - Lattitude * 10**7
    long lon;                ///< param 4 - Longitude * 10**7
};

class Location
{
public:
    Location() {} //empty constructor

    static void loc_update(struct Loc &loc, float bearing, float distance);
    static void loc_offset(struct Loc &loc, float ofs_north, float ofs_east);

private:
    static float _longitude_scale(const struct Loc &loc);

};

#endif // LOCATION_H
