#include "logformat.h"

LogFormat::LogFormat():
    m_CamSet(false),
    m_GpsSet(false)
{

}

bool LogFormat::isReady()
{
    if(m_CamSet && m_GpsSet) return true;
    else return false;
}

void LogFormat::setFormat(QString formatLine)
{
    QStringList format = formatLine.split(',');
    if(format[3].trimmed() == "CAM")
    {

        for(int i = 0; i < format.size(); ++i)
        {
            if(format[i].trimmed() == "GPSTime") m_CamFormat[0] = i-4;
            else if(format[i].trimmed() == "Roll") m_CamFormat[1] = i-4;
            else if(format[i].trimmed() == "Pitch") m_CamFormat[2] = i-4;
            else if(format[i].trimmed() == "Yaw") m_CamFormat[3] = i-4;
        }
        m_CamSet = true;
    }
    else if(format[3].trimmed() == "GPS")
    {
        for(int i = 0; i < format.size(); ++i)
        {
            if(format[i].trimmed() == "Lat") m_GpsFormat[0] = i-4;
            else if(format[i].trimmed() == "Lng") m_GpsFormat[1] = i-4;
            else if(format[i].trimmed() == "Alt") m_GpsFormat[2] = i-4;
            else if(format[i].trimmed() == "Spd") m_GpsFormat[3] = i-4;
            else if(format[i].trimmed() == "GCrs") m_GpsFormat[4] = i-4;
        }
        m_GpsSet = true;
    }
}

double LogFormat::getRollFromCAM(QString camLine)
{
    QStringList slist = camLine.split(',');
    return slist[m_CamFormat[ROLL_d]].toDouble();
}

double LogFormat::getPitchFromCAM(QString camLine)
{
    QStringList slist = camLine.split(',');
    return slist[m_CamFormat[PITCH_d]].toDouble();
}

double LogFormat::getYawFromCAM(QString camLine)
{
    QStringList slist = camLine.split(',');
    return slist[m_CamFormat[YAW_d]].toDouble();
}

long LogFormat::getTimeFromCAM(QString camLine)
{
    QStringList slist = camLine.split(',');
    return slist[m_CamFormat[TIME_d]].toLong();
}

double LogFormat::getLatitudeFromGPS(QString gpsLine)
{
    QStringList slist = gpsLine.split(',');
    return slist[m_GpsFormat[LAT_d]].toDouble();
}

double LogFormat::getLongitudeFromGPS(QString gpsLine)
{
    QStringList slist = gpsLine.split(',');
    return slist[m_GpsFormat[LON_d]].toDouble();
}

double LogFormat::getAltitudeFromGPS(QString gpsLine)
{
    QStringList slist = gpsLine.split(',');
    return slist[m_GpsFormat[ALT_d]].toDouble();
}

double LogFormat::getGroundSpeedFromGPS(QString gpsLine)
{
    QStringList slist = gpsLine.split(',');
    return slist[m_GpsFormat[GS_d]].toDouble();
}

double LogFormat::getCourseOverGroundFromGPS(QString gpsLine)
{
    QStringList slist = gpsLine.split(',');
    return slist[m_GpsFormat[CoG_d]].toDouble();
}

