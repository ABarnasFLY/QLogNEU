#include "converterthread.h"

void ConverterThread::setConverter(QBinToLog *value)
{
    m_converter = value;
}

void ConverterThread::run()
{
    if(m_converter)
        m_converter->exec();
}

