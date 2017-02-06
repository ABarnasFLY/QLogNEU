#ifndef LOGFORMAT_H
#define LOGFORMAT_H

#include <QString>


class LogFormat
{
    int m_CamFormat[4];
    int m_GpsFormat[5];
    bool m_CamSet;
    bool m_GpsSet;
public:
    bool formatReady();
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
