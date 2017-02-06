#include "logformat.h"

LogFormat::LogFormat():
    m_CamSet(false),
    m_GpsSet(false)
{

}

bool LogFormat::formatReady()
{
    if(m_CamSet || m_GpsSet) return true;
    else return false;
}

void LogFormat::setFormat(QString formatLine)
{
    QStringList format = formatLine.split(',');

    if(format[3].trimmed() == "CAM")
    {
        for(int i = 0; i < formatLine.size(); ++i)
        {
            if(format[i].trimmed() == "GPSTime") m_CamFormat[0] = i-5;
            else if(format[i].trimmed() == "Roll") m_CamFormat[1] = i-5;
            else if(format[i].trimmed() == "Pitch") m_CamFormat[2] = i-5;
            else if(format[i].trimmed() == "Yaw") m_CamFormat[3] = i-5;
        }
        m_CamSet = true;
    }
    else if(format[3].trimmed() == "GPS")
    {
        for(int i = 0; i < formatLine.size(); ++i)
        {
            if(format[i].trimmed() == "Lat") m_GpsFormat[0] = i-5;
            else if(format[i].trimmed() == "Lng") m_GpsFormat[1] = i-5;
            else if(format[i].trimmed() == "Alt") m_GpsFormat[2] = i-5;
            else if(format[i].trimmed() == "Spd") m_GpsFormat[3] = i-5;
            else if(format[i].trimmed() == "GCrs") m_GpsFormat[4] = i-5;
        }
        m_GpsSet = true;
    }
}

double LogFormat::getRollFromCAM(QString camLine)
{

}

double LogFormat::getPitchFromCAM(QString camLine)
{

}

double LogFormat::getYawFromCAM(QString camLine)
{

}

long LogFormat::getTimeFromCAM(QString camLine)
{

}

double LogFormat::getLatitudeFromGPS(QString gpsLine)
{

}

double LogFormat::getLongitudeFromGPS(QString gpsLine)
{

}

double LogFormat::getAltitudeFromGPS(QString gpsLine)
{

}

double LogFormat::getGroundSpeedFromGPS(QString gpsLine)
{

}

double LogFormat::getCourseOverGroundFromGPS(QString gpsLine)
{

}

