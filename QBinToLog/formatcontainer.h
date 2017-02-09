#ifndef FORMATCONTAINER_H
#define FORMATCONTAINER_H
#include <QString>
#include <QVector>
struct FMT
{
    char name[5];
    char format[17];
    char labels[65];
};

class FormatContainer
{
public:
    FormatContainer();
    QString toQString();
    void fillFMT(const char*);                      //function recives a poiter to line get from file, gets and fill data in class containers
    int findFormat(unsigned char id);         //function recives message id and returns iterator of proper formating data
    FMT getFormat(int iterator);              //function recives iterator of proper data and returns a structure
    int getLength(unsigned char id);
private:
    QVector <unsigned char> m_ids;
    QVector <unsigned char> m_lengths;
    QVector <FMT> m_table;
};

#endif // FORMATCONTAINER_H
