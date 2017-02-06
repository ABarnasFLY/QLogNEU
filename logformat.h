#ifndef LOGFORMAT_H
#define LOGFORMAT_H

#define TIME_d 0
#define ROLL_d 1
#define PITCH_d 2
#define YAW_d 3


#define LAT_d 0
#define LON_d 1
#define ALT_d 2
#define GS_d 3
#define CoG_d 4


#include "global_defs.h"



class LogFormat
{
    int m_CamFormat[4];
    int m_GpsFormat[5];
    bool m_CamSet;
    bool m_GpsSet;
public:
    bool isReady();
    void setFormat(QString formatLine);
    double getRollFromCAM(QString camLine);
    double getPitchFromCAM(QString camLine);
    double getYawFromCAM(QString camLine);
    long getTimeFromCAM(QString camLine);
    double getLatitudeFromGPS(QString gpsLine);
    double getLongitudeFromGPS(QString gpsLine);
    double getAltitudeFromGPS(QString gpsLine);
    double getGroundSpeedFromGPS(QString gpsLine);
    double getCourseOverGroundFromGPS(QString gpsLine);
    LogFormat();
};

#endif // LOGFORMAT_H
