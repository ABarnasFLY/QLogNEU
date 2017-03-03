#ifndef PARSERRTK_H
#define PARSERRTK_H
#include "parsemachine.h"
#include "rinexparser.h"
#include "rtklogparser.h"
#include "rtkpostprocessparser.h"

class ParserRTK : public ParseMachine
{
    RinexParser m_rinex;
    RTKLogParser m_log;
    RTKPostProcessParser m_ppRTK;
public:
    ParserRTK(QString logPath, QString rinexPath, QString posPath, QObject *parent);
    void run();
};

#endif // PARSERRTK_H
