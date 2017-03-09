#include "parserrtk.h"
ParserRTK::ParserRTK(QString logPath, QString rinexPath, QString posPath, QObject *parent):
    ParseMachine(logPath, parent),
    m_rinex(&m_vectCamLog, rinexPath),
    m_ppRTK(posPath, &m_vectCamLog),
    m_log(&m_logFile, &m_vectCamLog)
{

}

void ParserRTK::run()
{
    connect(&m_log,SIGNAL(updateProgress(int)),this,SLOT(onUpdateProgress(int)));
    connect(&m_log,SIGNAL(setProgressMax(int)),this,SLOT(onSetMaxProgress(int)));
    emit sendMessage("Processing RINEX");
    m_rinex.run();
    emit sendMessage("Looking for coordinetes");
    m_ppRTK.run();
    emit sendMessage("Fit details");
    m_log.run();

}
