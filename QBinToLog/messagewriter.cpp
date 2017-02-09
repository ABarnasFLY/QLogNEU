#include "messagewriter.h"

MessageWriter::MessageWriter()
{
    m_fileIsOpen = 0;
}
MessageWriter::MessageWriter(QString dir)
{
    m_fileTo = new QFile(dir);
    m_stream = new QTextStream(m_fileTo);
    if(m_fileTo->open(QIODevice::ReadWrite | QIODevice::Text))
    {
        m_fileIsOpen = 1;
    }
    else m_fileIsOpen = 0;
}
MessageWriter::~MessageWriter()
{
    delete m_fileTo;
    delete m_stream;
}

void MessageWriter::openFile(QString dir)
{
    m_fileTo->setFileName(dir);
    if(m_fileTo->open(QIODevice::ReadWrite | QIODevice::Text))
    {
        m_fileIsOpen = 1;
        m_stream->setDevice(m_fileTo);
    }
    else    m_fileIsOpen = 0;
}
void MessageWriter::toFile(QString message)
{
    if(message != 0)
    {
        message.push_back('\n');
        *m_stream << message;
    }
}

bool MessageWriter::isFileOpen() const
{
    return m_fileIsOpen;
}
