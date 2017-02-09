#include "messagereader.h"

MessageReader::MessageReader()
{
    m_fileIsOpen = 0;
    eof = 0;
}
MessageReader::MessageReader(QString dir)
{
    m_formats = new FormatContainer();
    m_fileFrom = new QFile(dir);
    if(m_fileFrom->open(QIODevice::ReadOnly))
    {
        m_fileIsOpen = 1;
        m_size = m_fileFrom->size();

    }
    else m_fileIsOpen = 0;
    eof = 0;
    m_size = m_fileFrom->size();
}
MessageReader::MessageReader(QFile *file)
{
    m_formats = new FormatContainer();
    m_fileFrom = file;
    if(m_fileFrom->open(QIODevice::ReadOnly))
    {
        m_fileIsOpen = 1;
        m_size = m_fileFrom->size();

    }
    else m_fileIsOpen = 0;
    eof = 0;
}

MessageReader::~MessageReader()
{
    delete m_formats;
}

const char* MessageReader::readLine()
{
    if(m_fileFrom->atEnd()) return "X";
    int step = 0;
    char byte;
    bool end = 0;
    int position = m_fileFrom->pos();
    while(!end)
    {m_fileFrom->read(&byte,1);
    switch(step)
    {
    case 0:
        if(byte == -93) step++;break;
    case 1:
        if(byte == -107) step++;break;
    case 2:
    {
        if(byte == -128)
        {
            m_fileFrom->read(&byte,1);
            if(byte == -128)
            {
            m_fileFrom->read(&byte,1);
            m_fileFrom->seek(position);
            char *line = new char[(int)byte];
            for(int i = 0; i < byte; i++) line[i] = 0;
            m_fileFrom->read(line,(unsigned int)byte);
            pos = m_fileFrom->pos();
            return line;
            }
            else
            {
                m_fileFrom->seek(m_fileFrom->pos()-2);
                m_fileFrom->read(&byte,1);
                int length = m_formats->getLength((unsigned char)byte);
                char *line = new char[length];
                for(int i = 0; i < length; i++) line[i] = 0;
                m_fileFrom->seek(position);
                m_fileFrom->read(line,length);
                pos = m_fileFrom->pos();
                return line;
            }
        }
        else
        {
            int length = m_formats->getLength((unsigned char) byte);
            char *line = new char[length];
            for(int i = 0; i < length; i++) line[i] = 0;
            m_fileFrom->seek(position);
            m_fileFrom->read(line,length);
            pos = m_fileFrom->pos();
            return line;

        }

    }

    }
    }
    return "ERROR";
}

int MessageReader::size() const
{
    return m_size;
}
void MessageReader::openFile(QString dir)
{
    m_fileFrom->setFileName(dir);
    if(m_fileFrom->open(QIODevice::ReadOnly))   m_fileIsOpen = 1;
    else    m_fileIsOpen = 0;
}
QString MessageReader::toString()
{
    QString message;
    const char *line = readLine();
    if (line[0]== 'X')
    {
        eof = 1;
        return 0;
    }
    //int id = (unsigned char) line[2];
    int i  = m_formats->findFormat(line[2]);
    if(line[2] == -128)
    {
        m_formats->fillFMT(line);
    }
    message += m_formats->getFormat(i).name;
    line += 3;
    for(int j = 0; j < 16; j++)
    {
        switch(m_formats->getFormat(i).format[j])
        {
        case 'N':
        {

            char text[17];
            for(int it = 0; it < 16; it++) text[it] = line[it];
            text[16] = 0;
            message += ", "; message +=text ;
            line+=16;
            break;
        }
        case 'n':
        {
            char text[5];
            for(int it = 0; it < 4; it++) text[it] = line[it];
            text[4] = 0;
            message += ", "; message +=text ;
            line+=4;
            break;
        }
        case 'f':
        {
            float *f = (float*) line;
            message += ", " + QString::number(*f,'g',7) ;
            line+=4; break;
        }
        case 'b':
        {
            qint8 *value = (qint8*) line ;
            message += ", " + QString::number(*value) ;
            line++;
            break;
        }
        case 'B':
        {
            quint8 *value = (quint8*) line;
            message += ", " + QString::number(*value);
            line++;
            break;
        }
        case 'h':
        {
            qint16 *value = (qint16*) line;
            message += ", " + QString::number(*value) ;
            line+=2;
            break;
        }
        case 'H':
        {
            quint16 *value = (quint16*) line;
            message += ", " + QString::number(*value);
            line+=2;
            break;
        }
        case 'i':
        {
            qint32 *value = (qint32*) line;
            message += ", " + QString::number(*value) ;
            line+=4;
            break;
        }
        case 'I':
        {
            quint32 *value = (quint32*) line;
            message += ", " + QString::number(*value) ;
            line+=4;
            break;
        }
        case 'd':
        {
            double *value = (double *) line;
            message += ", " + QString::number(*value,'g',7) ;
            line+=sizeof(double);
            break;
        }
        case 'Z':
        {
            char text[65];
            for(int it = 0; it < 64; it++) text[it] = line[it];
            text[64] = 0;
            line += 64;
            message += ", "; message += text;
            break;
        }
        case 'c':
        {
            qint16 *number = (qint16*) line;
            double value = *number / 100.0;
            message += ", " + QString::number(value,'g',7);
            line+=2;
            break;
        }
        case 'C':
        {
            quint16 *number = (quint16*) line;
            double value = *number / 100.0;
            message += ", " + QString::number(value,'g',7);
            line+=2;
            break;
        }
        case 'e':
        {
            qint32 *number = (qint32*) line;
            double value = *number / 100.0;
            message += ", " + QString::number(value,'g',7);
            line+=4;
            break;
        }
        case 'E':
        {
            quint32 *number = (quint32*) line;
            double value = *number / 100.0;
            message += ", " + QString::number(value,'g',7);
            line+=2;
            break;
        }
        case 'L':
        {
            qint32 *number = (qint32*) line;
            double value = *number / 10000000.0;
            message += ", " + QString::number( value,'g',8);
            line+=4;
            break;
        }
        case 'q':
        {
            qint64 *value = (qint64*) line;
            message += ", " + QString::number(*value) ;
            line+=8;
            break;
        }
        case 'Q':
        {
            quint64 *value = (quint64*) line;
            message += ", " + QString::number(*value) ;
            line+=8;
            break;
        }
         case 0:
        {
            return message;
        }
        }

    }
    return "";
}

bool MessageReader::isFileOpen() const
{
    return m_fileIsOpen;
}

bool MessageReader::isQValued()
{
    if(m_formats->getFormat(2).format[0] == 'Q') return true;
    return false;
}

