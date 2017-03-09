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


    LogFormat m_format;
    NearLogs_t m_nearLogs;
    int picDone;
    bool eof;
    QTextStream m_logFileStream;
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
protected:
    QFile m_logFile;
    QVector<CamLog_t> m_vectCamLog;
public:
    ///////////////////////////////
    /// \brief ParseMachine
    /// \param picPath
    /// Parser constructor, picPath is obligatory
    ParseMachine(QString logPath , QObject* parent);

    bool getEof() const;
    void setEof(bool value);

    ///////////////////////////////
    /// \brief run
    /// Parse log and create vector of pics data
    virtual void run();
    int getPicDone() const;

    QVector<CamLog_t> getVectCamLog() const;
protected slots:
    void onUpdateProgress(int);
    void onSetMaxProgress(int);
private:
signals:
    void sendMessage(QString);
    void maxProgresValue(int);
    void currentProgress(int);

};

#endif // PARSEMACHINE_H
