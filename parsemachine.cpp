#include "parsemachine.h"

ParseMachine::ParseMachine():
    m_logFile("/home/office/QLogNEU/37.log"),
    picDone(0)
{

}

CamLog_t ParseMachine::decodeLines(NearLogs_t nearlogs)
{
    CamLog_t localLog;
    localLog.alt = m_format.getAltitudeFromGPS(nearlogs.lastGPS);
    localLog.CoG = m_format.getCourseOverGroundFromGPS(nearlogs.lastGPS);
    localLog.lat = m_format.getLatitudeFromGPS(nearlogs.lastGPS);
    localLog.lon = m_format.getLongitudeFromGPS(nearlogs.lastGPS);
    localLog.GS  = m_format.getGroundSpeedFromGPS(nearlogs.lastGPS);

    localLog.time  = m_format.getTimeFromCAM(nearlogs.CAM);
    localLog.roll  = m_format.getRollFromCAM(nearlogs.CAM);
    localLog.pitch = m_format.getPitchFromCAM(nearlogs.CAM);
    localLog.yaw   = m_format.getYawFromCAM(nearlogs.CAM);

    return localLog;
}

void ParseMachine::run()
{
    State_line state = INIT;
    while(!m_data.getEof())
    {
        switch (state) {
        case INIT:
            state = FL_on_Init();
            break;
        case FILL_FORMAT_TABLE:
            state = FL_on_FillFormatTable();
            break;
        case SEARCHING_FOR_CAM:
            state = FL_on_SearchingForCam();
            break;
        case FOUND_CAM:
            state = FL_on_FoundCam();
            break;
        case FOUND_GPS:
            state = FL_on_FoundGPS();
            break;
        case FOUND_ATT:
            state = FL_on_FoundAtt();
            break;
        case PIC_DONE:
            state = FL_on_PicDone();
            break;
        case END_OF_LOG:
            m_data.setEof(true);
            break;
        }
    }
    print();
}

FLSTATE ParseMachine::FL_on_Init()
{
    m_logFile.open(QIODevice::ReadOnly);
    m_logFileStream.setDevice(&m_logFile);
    return FILL_FORMAT_TABLE;
}

FLSTATE ParseMachine::FL_on_FillFormatTable()
{
    while(!m_logFileStream.atEnd())
    {
        QString line;
        line = m_logFileStream.readLine();
        QStringRef label = line.leftRef(3);
        if (label.startsWith("FMT"))
        {
           m_format.setFormat(line);
        }
        if(m_format.isReady()) return SEARCHING_FOR_CAM;
    }
    return END_OF_LOG;
}

FLSTATE ParseMachine::FL_on_SearchingForCam()
{
    while(!m_logFileStream.atEnd())
    {
        QString line;
        line = m_logFileStream.readLine();
        QStringRef label = line.leftRef(3);
        if(label.startsWith("CAM"))
        {
            m_nearLogs.CAM = line;
            return FOUND_CAM;
        }
        else if(label.startsWith("GPS"))
        {
            m_nearLogs.lastGPS = line;
        }
        else if(label.startsWith("ATT"))
        {
            m_nearLogs.lastATT = line;
        }
    }
    return END_OF_LOG;
}

FLSTATE ParseMachine::FL_on_FoundCam()
{
    while(!m_logFileStream.atEnd())
    {
        QString line;
        line = m_logFileStream.readLine();
        QStringRef label = line.leftRef(3);
        if(label.startsWith("GPS"))
        {
            m_nearLogs.nextGPS = line;
            return FOUND_GPS;
        }
        else if(label.startsWith("ATT"))
        {
            m_nearLogs.nextATT = line;
            return FOUND_ATT;
        }
    }
    return END_OF_LOG;
}

FLSTATE ParseMachine::FL_on_FoundGPS()
{
    while(!m_logFileStream.atEnd())
    {
        QString line;
        line = m_logFileStream.readLine();
        QStringRef label = line.leftRef(3);
        if(label.startsWith("ATT"))
        {
            m_nearLogs.nextATT = line;
            return PIC_DONE;
        }
    }
    return END_OF_LOG;
}

FLSTATE ParseMachine::FL_on_FoundAtt()
{
    while(!m_logFileStream.atEnd())
    {
        QString line;
        line = m_logFileStream.readLine();
        QStringRef label = line.leftRef(3);
        if(label.startsWith("GPS"))
        {
            m_nearLogs.nextGPS = line;
            return PIC_DONE;
        }
    }
    return END_OF_LOG;
}

FLSTATE ParseMachine::FL_on_PicDone()
{
    m_vectCamLog.push_back(decodeLines(m_nearLogs));
    picDone++;
    return SEARCHING_FOR_CAM;
}

ParseData::ParseData():
eof(false)
{

}

bool ParseData::getEof() const
{
    return eof;
}

void ParseData::setEof(bool value)
{
    eof = value;
}


int ParseMachine::getPicDone() const
{
    return picDone;
}

void ParseMachine::print()
{
    QFile file("/home/office/QLogNEU/printOut.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream str(&file);

    for(int i = 0; i<m_vectCamLog.size(); i++)
    {
        str << m_vectCamLog[i].alt << " | " << m_vectCamLog[i].lat << " | " << m_vectCamLog[i].lon << " | " << m_vectCamLog[i].roll << " | " << m_vectCamLog[i].pitch << " | " << m_vectCamLog[i].yaw << '\n';

               /*
                * typedef struct {
           unsigned long	time;
           float			alt;
           double          lat;
           double          lon;
           float           roll;
           float           pitch;
           float           yaw;
           float			CoG;		//GPS Course over Ground
           float			GS;			//GPS Ground Speed [m/s]
       } CamLog_t;*/
    }
    file.close();
}
