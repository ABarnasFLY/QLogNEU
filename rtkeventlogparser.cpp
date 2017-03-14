#include "rtkeventlogparser.h"

RTKEventLogParser::RTKEventLogParser(QString path, QVector<CamLog_t> *vectCamLog):
    m_file(path),
    m_vectCamLog(vectCamLog)
{

}

void RTKEventLogParser::run()
{
    m_file.open(QIODevice::ReadOnly);
    QTextStream posStream(&m_file);
    QString line = "%";
    QStringList lineSplited;
    while(line.startsWith('%'))
    {
        line = posStream.readLine();
    } // skip header

    while(!posStream.atEnd())
    {
        CamLog_t logEntry;
        line = posStream.readLine();
        lineSplited = line.split(' ', QString::SkipEmptyParts);
        QStringList date = lineSplited[0].split('/');
        QStringList time = lineSplited[1].split(':');
        logEntry.time = timeUtils::convTimeUTCToGPS(date[0], date[1], date[2], time[0], time[1], time[2]);
        if(lineSplited[5].startsWith("1") && lineSplited[14].startsWith("999"))
            logEntry.Q = 1;
        else
            logEntry.Q = 2;
        logEntry.lat = lineSplited[2].toDouble();
        logEntry.lon = lineSplited[3].toDouble();
        logEntry.alt = lineSplited[4].toFloat() - 0.12;
        m_vectCamLog->push_back(logEntry);
    }
}
