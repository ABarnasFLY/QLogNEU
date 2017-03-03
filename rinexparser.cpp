#include "rinexparser.h"

RinexParser::RinexParser(QVector<CamLog_t> *vectCamLog, QString rinexPath):
    m_vectCamLog(vectCamLog),
    m_rinex(rinexPath)
{

}

void RinexParser::run()
{
    QString bufor1, bufor2;
    QStringList line;
    CamLog_t logEntry;
    m_rinex.open(QIODevice::ReadOnly);
    QTextStream ts(&m_rinex);
    bool z = false;
    int i = 0;
    m_vectCamLog->clear();
    while(!ts.atEnd())
    {
        bufor1 = m_rinex.readLine();
        if(bufor1[0] == '>')
        {
            if(z)
            {
                line = bufor2.split(' ');
                int size = line.size();
                int step = 0;
                for(int j = 0; j < size; ++j)
                {
                    if(line[step]=="") line.removeAt(step);
                    else step++;
                }
                logEntry.time = timeUtils::convTimeUTCToGPS(line[1],line[2],line[3],line[4],line[5],line[6]);
                m_vectCamLog->push_back(logEntry);
                ++i;
            }
            else
            {
                z = true;
                bufor2 = bufor1;
            }
        }
        else z = false;
    }
}
