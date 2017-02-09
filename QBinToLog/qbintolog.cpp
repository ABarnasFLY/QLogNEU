#include "qbintolog.h"


QBinToLog::QBinToLog(QString uFrom, QString uTo)
{
    m_reader = new MessageReader(uFrom);
    m_writer = new MessageWriter(uTo);
    m_urlFrom = uFrom;
    m_urlTo = uTo;
    if(m_writer->isFileOpen() & m_reader->isFileOpen())
    m_ready = 1;
}
QBinToLog::~QBinToLog()
{
    delete m_reader;
    delete m_writer;
}

void QBinToLog::exec()
{
    if(m_ready)
    {
        int it = 0;
        emit setProgressBar(m_reader->size());
        while(!m_reader->eof)
        {
            m_writer->toFile(m_reader->toString());
            it++;
            if(it > 100)
            {
                emit refreshProgressBar(m_reader->pos);
                it = 0;
            }
        }
    }
}

