#include "location.h"

// radius of earth in meters
#define RADIUS_OF_EARTH 6378100

// scaling factor from 1e-7 degrees to meters at equater
// == 1.0e-7 * DEG_TO_RAD * RADIUS_OF_EARTH
#define LOCATION_SCALING_FACTOR 0.011131884502145034f
// inverse of LOCATION_SCALING_FACTOR
#define LOCATION_SCALING_FACTOR_INV 89.83204953368922f

/*
 *  extrapolate latitude/longitude given bearing and distance
 * Note that this function is accurate to about 1mm at a distance of
 * 100m. This function has the advantage that it works in relative
 * positions, so it keeps the accuracy even when dealing with small
 * distances and floating point numbers
 */

void
Location::loc_update(Loc &loc, float bearing, float distance)
{
    float ofs_north = cosf(radians(bearing))*distance;
    float ofs_east  = sinf(radians(bearing))*distance;
    loc_offset(loc, ofs_north, ofs_east);
}

/*
 *  extrapolate latitude/longitude given distances north and east
 */
void
Location::loc_offset(Loc &loc, float ofs_north, float ofs_east)
{
    if (ofs_north != 0 || ofs_east != 0) {
        long dlat = ofs_north * LOCATION_SCALING_FACTOR_INV;
        long dlng = (ofs_east * LOCATION_SCALING_FACTOR_INV) / _longitude_scale(loc);
        loc.lat += dlat;
        loc.lon += dlng;
    }
}

float
Location::_longitude_scale(const struct Loc &loc)
{
    static long last_lat;
    static float scale = 1.0;
    if (abs(last_lat - loc.lat) < 100000) {
        // we are within 0.01 degrees (about 1km) of the
        // same latitude. We can avoid the cos() and return
        // the same scale factor.
        return scale;
    }
    scale = cosf(radians(loc.lat * 1.0e-7f));
    last_lat = loc.lat;
    return scale;
}
