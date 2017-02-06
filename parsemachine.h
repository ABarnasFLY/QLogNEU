#ifndef PARSEMACHINE_H
#define PARSEMACHINE_H

#include <QFile>
#include <QTextStream>
#include <QVector>
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
    NearLogs_t m_nearLogs;
    QVector<CamLog_t> m_vectCamLog;
    int picDone;
    void print();

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
        void run();
        int getPicDone() const;

private:
        CamLog_t decodeLines(NearLogs_t nearlogs);
};

#endif // PARSEMACHINE_H
