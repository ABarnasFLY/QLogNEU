#include "rtkpostprocessparser.h"

RTKPostProcessParser::RTKPostProcessParser(QString path, QVector<CamLog_t> *vectCamLog):
    m_file(path),
    m_vectCamLog(vectCamLog)
{

}

void RTKPostProcessParser::run()
{
    m_file.open(QIODevice::ReadOnly);
    QTextStream posStream(&m_file);
    QString line = "%";
    QStringList curentLine, lastLine;
    unsigned long int timeOfLastLine= 0, timeOfCurentline= 0;
    while(line.startsWith('%'))
    {
       line = posStream.readLine();
    } // skip header
    QVector<CamLog_t>::Iterator it = m_vectCamLog->begin();
    while(it!=m_vectCamLog->end() && ! posStream.atEnd())
    {
        line = posStream.readLine();
        curentLine = line.split(' ', QString::SkipEmptyParts);
        QStringList date = curentLine[0].split('/');
        QStringList time = curentLine[1].split(':');
        timeOfCurentline = timeUtils::convTimeUTCToGPS(date[0], date[1], date[2], time[0], time[1], time[2]);
        if(timeOfCurentline > it->time)
        {
            if(curentLine[5].startsWith("1") && lastLine[5].startsWith("1"))
                it->Q = 1;
            else
                it->Q = 2;
            double lat = curentLine[2].toDouble();
            double lon = curentLine[3].toDouble();
            float alt = curentLine[4].toFloat();
            double llat = lastLine[2].toDouble();
            double llon = lastLine[3].toDouble();
            float lalt = lastLine[4].toFloat();
            it->lat = llat + (lat - llat)* (((double)it->time - (double)timeOfLastLine)/ ((double)timeOfCurentline - (double)timeOfLastLine));
            it->lon = llon + (lon - llon)* (((double)it->time - (double)timeOfLastLine)/ ((double)timeOfCurentline - (double)timeOfLastLine));
            it->alt = lalt + (alt - lalt) * (((double)it->time - (double)timeOfLastLine)/ ((double)timeOfCurentline - (double)timeOfLastLine));
            if(it != m_vectCamLog->end())  it++;
        }
        timeOfLastLine = timeOfCurentline;
        lastLine =  curentLine;
    }
}
