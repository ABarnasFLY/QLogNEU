#ifndef PARSEMACHINE_H
#define PARSEMACHINE_H

#include <QFile>
#include <QTextStream>
#include "logformat.h"
#define FLSTATE ParseMachine::State_line

class ParseData
{
    bool eof;
public:
    ParseData();
    bool getEof() const;
    void setEof(bool value);
};

class ParseMachine
{
    QFile m_logFile;
    QTextStream m_logFileStream;
    LogFormat m_format;
    ParseData m_data;
    void findLine();
    enum State_line
    {
        INIT,
        FILL_FORMAT_TABLE,
        SEARCHING_FOR_CAM,
        FOUND_CAM,
        FOUND_GPS,
        FOUND_ATT,
        PIC_DONE,
        END_OF_LOG
    };
    State_line FL_on_Init();
    State_line FL_on_SearchingForCam();
    State_line FL_on_FoundCam();
    State_line FL_on_FoundGPS();
    State_line FL_on_FoundAtt();
    State_line FL_on_PicDone();
    State_line FL_on_FillFormatTable();
public:
    ParseMachine();
private:
    void DecodeCAMFormat(QStringList formatLine);
    void DecodeGPSFormat(QStringList formatLine);
    bool FormatValid(int ptr[], int sizeFmt, int lineSize);
};

#endif // PARSEMACHINE_H
