#include "parsemachine.h"
#define CONDITION true //temporary condition
ParseMachine::ParseMachine()
{

}

void ParseMachine::findLine()
{
    State_line state = INIT;
    while(!m_data.getEof())
    {
        switch (state) {
        case INIT:
            state = on_FL_Init();
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
}

FLSTATE ParseMachine::FL_on_Init()
{
    m_logFile.open("filename", QIODevice::ReadOnly);
    m_logFileStream(&m_logFile);
    return FILL_FORMAT_TABLE;
}

FLSTATE ParseMachine::FL_on_FillFormatTable()
{
    while(!m_logFileStream.atEnd())
    {
        QString line;
        line = logFile.readLine();
        QStringRef label = line.leftRef(3);
        if (lastPos != 0 && label.startsWith("FMT"))
        {
           m_format.setFormat(line);
        }

    }
    return END_OF_LOG;
}

FLSTATE ParseMachine::FL_on_SearchingForCam()
{
    //ReadLine
    //Save last GPS & ATT
    //Branch to Found Cam if line is CAM
    while(!m_logFileStream.atEnd())
    {

    }
    return END_OF_LOG;
}

FLSTATE ParseMachine::FL_on_FoundCam()
{
    //ReadLine
    //Branch to Found GPS if line is GPS
    //branch to Found ATT if line is ATT
    while(!m_logFileStream.atEnd())
    {

    }
    return END_OF_LOG;
}

FLSTATE ParseMachine::FL_on_FoundGPS()
{
    //ReadLine
    //Branch to PIC DONE if line is ATT
    while(!m_logFileStream.atEnd())
    {

    }
    return END_OF_LOG;
}

FLSTATE ParseMachine::FL_on_FoundAtt()
{
    //ReadLine
    //Branch to PIC DONE if line is GPS
    while(!m_logFileStream.atEnd())
    {

    }
    return END_OF_LOG;
}

FLSTATE ParseMachine::FL_on_PicDone()
{
    //Read all lines to vector
    //Decode and fill waht need to be filled
    while(!m_logFileStream.atEnd())
    {

    }
    return END_OF_LOG;
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

bool ParseMachine::FormatValid(int ptr[], int sizeFmt, int lineSize)
{
    for(int i = 0; i < sizeFmt; ++i)
    {
        if(ptr[i] >= lineSize) return false;
    }
    return true;
}
