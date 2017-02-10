#ifndef PARSEMACHINE_H
#define PARSEMACHINE_H

#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QObject>
#include "logformat.h"

#define PARSERSTATE ParseMachine::State_line



class ParseMachine : public QObject
{
    Q_OBJECT

    QFile m_logFile;
    QTextStream m_logFileStream;
    LogFormat m_format;
    NearLogs_t m_nearLogs;
    QVector<CamLog_t> m_vectCamLog;
    int picDone;
    bool eof;

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
    //State handlers////////////////
    State_line on_Init();
    State_line on_SearchingForCam();
    State_line on_FoundCam();
    State_line on_FoundGPS();
    State_line on_FoundAtt();
    State_line on_PicDone();
    State_line on_FillFormatTable();
    ////////////////////////////////

    CamLog_t decodeLines(NearLogs_t nearlogs);

    ///////////////////////////////
    /// \brief print
    /// Print some data from vector for debug purpose
    void print();
public:
    ///////////////////////////////
    /// \brief ParseMachine
    /// \param picPath
    /// Parser constructor, picPath is obligatory
    ParseMachine(QString picPath , QObject* parent);

    bool getEof() const;
    void setEof(bool value);

    ///////////////////////////////
    /// \brief run
    /// Parse log and create vector of pics data
    void run();
    int getPicDone() const;

    QVector<CamLog_t> getVectCamLog() const;

private:

};

#endif // PARSEMACHINE_H
