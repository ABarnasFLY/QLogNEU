#include "rtklogparser.h"
RTKLogParser::RTKLogParser(QFile *log, QVector<CamLog_t> *vec):
    m_log(log),
    m_vectCamLog(vec),
    m_logStream(log)
{

}

void RTKLogParser::run()
{
    bool end = false;
    State state = INIT;
    while(!end)
    {
        switch (state) {
        case INIT:
            state = on_Init();
            break;
        case FORMAT_DETECTION:
            state = on_FormatDetection();
            break;
        case SYNCHRONIZATION:
            state = on_Synchronization();
            break;
        case CONVERSION:
            state = on_Conversion();
            break;
        case SEARCHING:
            state = on_Searching();
            break;
        case DONE:
            end = true;
            break;
        case ERROR:
            on_Error(); // TODO
            end = true;
            break;
        }
    }
}

PSTATE RTKLogParser::on_Init()
{
    m_GPSTimePos = 0;
    m_rollPos = 0;
    m_pitchPos = 0;
    m_yawPos = 0;
    m_it_vectCamLog = m_vectCamLog->begin();
    m_log->open(QIODevice::ReadOnly);
    m_currentTimeMark = 0;
    return FORMAT_DETECTION;
}

PSTATE RTKLogParser::on_FormatDetection()
{
    QString line;
    QStringList lineList;
    int found = 0; // 1 - GPSms 2-Roll 4-Pitch 8-Yaw
    while(!m_logStream.atEnd())
    {
        line = m_logStream.readLine();
        lineList = line.split(',', QString::SkipEmptyParts);
        if(lineList[0] == "FMT" )
        {
            if(lineList[3] == " GPS")
            {
                for (int i = 0; i < lineList.size(); i++)
                {
                    if(lineList[i] == "GMS")
                    {
                        m_GPSTimePos = i - 4;
                        found |=1;
                        break;
                    }
                }
            }
            else if(lineList[3] == " ATT")
            {
                for(int i = 0; i < lineList.size(); ++i)
                {
                    if(lineList[i] == "Roll")
                    {
                        m_rollPos = i - 4;
                        found |= 2;
                    }
                    if(lineList[i] == "Pitch")
                    {
                        m_pitchPos = i - 4;
                        found |= 4;
                    }
                    if(lineList[i] == "Yaw")
                    {
                        m_yawPos = i - 4;
                        found |= 8;
                    }
                }
            }
            else if(found == 0xf) return SYNCHRONIZATION;
        }
    }
    return ERROR;
}

PSTATE RTKLogParser::on_Synchronization()
{
    QString line;
    QStringList lineList;
    while(!m_logStream.atEnd())
    {
        line = m_logStream.readLine();
        lineList = line.split(',', QString::SkipEmptyParts);
        if(lineList[0] == "GPS" )
        {
            m_timeDifference = lineList[m_GPSTimePos].toLongLong() - lineList[1].toLongLong()/1000;
            return CONVERSION;
        }
    }
    return ERROR;
}

PSTATE RTKLogParser::on_Conversion()
{
    m_currentTimeMark = m_it_vectCamLog->time + m_timeDifference;
    if(m_it_vectCamLog != m_vectCamLog->end()) m_it_vectCamLog++;
    else return DONE;

    return SEARCHING;
}

PSTATE RTKLogParser::on_Searching()
{
    QString line;
    QStringList lineList;
    while(!m_logStream.atEnd())
    {
        line = m_logStream.readLine();
        lineList = line.split(',', QString::SkipEmptyParts);
        if(lineList[0] == "ATT")
        {
            if(lineList[1].toLongLong() / 1000 > m_currentTimeMark)
            {
                m_it_vectCamLog->roll = lineList[m_rollPos].toDouble();
                m_it_vectCamLog->pitch = lineList[m_pitchPos].toDouble();
                m_it_vectCamLog->yaw = lineList[m_yawPos].toDouble();
                return CONVERSION;
            }
        }
    }
    return ERROR;
}

PSTATE RTKLogParser::on_Error()
{

}


