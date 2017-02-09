#ifndef MESSAGEREADER_H
#define MESSAGEREADER_H
#include "formatcontainer.h"
#include <QFile>
#include <QDir>
class MessageReader
{
public:
    MessageReader();
    MessageReader(QString dir);
    MessageReader(QFile *);
    ~MessageReader();
    bool isQValued();
    void openFile(QString dir);
    QString toString();



    const char* readLine();
    bool eof;
    int pos;


    bool isFileOpen() const;

    int size() const;

private:
    int m_size;
    bool m_fileIsOpen;
    FormatContainer *m_formats;
    QFile *m_fileFrom;

};
#endif // MESSAGEREADER_H
