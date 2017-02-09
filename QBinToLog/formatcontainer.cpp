#include "formatcontainer.h"

FormatContainer::FormatContainer()
{

}
QString FormatContainer::toQString()
{
    QString FMTdata;
    for(int i = 0; i < m_ids.length(); i++)
    {
        FMTdata += "FMT, ";
        FMTdata += QString::number(m_ids[i]);
        FMTdata += ", ";
        FMTdata += QString::number(m_lengths[i]);
        FMTdata += ", ";
        FMTdata += m_table[i].name;
        FMTdata += ", ";
        FMTdata += m_table[i].format;
        FMTdata += ", ";
        FMTdata += m_table[i].labels;
        FMTdata += '\n';
    }
    return FMTdata;
}

void FormatContainer::fillFMT(const char * line)
{
    m_ids.push_back(line[3]);
    m_lengths.push_back(line[4]);
    FMT newType;
    for(int i = 0; i < 4; i++)
    {
        newType.name[i] = line[i + 5];
    }
    newType.name[4] = 0;
    for(int i = 0; i < 16; i ++)
    {
        newType.format[i] = line[i + 9];
    }
    newType.format[16] = 0;
    for(int i = 0; i < 64; i++)
    {
        newType.labels[i] = line[i + 25];
    }
    newType.labels[64] = 0;
    m_table.push_back(newType);
}
int FormatContainer::getLength(unsigned char id)
{
    for(int i = 0; i < m_ids.length(); i++)
    {
        if(id == m_ids[i]) return m_lengths[i];
    }
    return 0;
}
int FormatContainer::findFormat(unsigned char id)
{
    for(int i = 0; i < m_ids.length(); i++)
    {
        if(id == m_ids[i]) return i;
    }
    return 0;
}
FMT FormatContainer::getFormat(int iterator)
{
    return m_table[iterator];
}
