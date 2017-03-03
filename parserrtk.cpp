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
    m_rinex.run();
    m_ppRTK.run();
    m_log.run();
}
