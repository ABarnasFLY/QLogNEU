#include "parserrtk.h"
ParserRTK::ParserRTK(QString logPath, QString posPath, QObject *parent):
    ParseMachine(logPath, parent),
    m_event(posPath, &m_vectCamLog),
    m_log(&m_logFile, &m_vectCamLog)
{

}

void ParserRTK::run()
{
    connect(&m_log,SIGNAL(updateProgress(int)),this,SLOT(onUpdateProgress(int)));
    connect(&m_log,SIGNAL(setProgressMax(int)),this,SLOT(onSetMaxProgress(int)));
    emit sendMessage("Parsing event log");
    m_event.run();
    emit sendMessage("Fit angles");
    m_log.run();
}
