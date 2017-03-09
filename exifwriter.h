#ifndef EXIFWRITER_H
#define EXIFWRITER_H
#include <QString>
#include <string>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <exiv2/exiv2.hpp>
#include <cassert>

class ExifWriter
{
    QString m_rPath;
    QString m_dirPath;
    QString m_copyDir;
public:
    ExifWriter(QString raportPath, QString dirPath, QString copyDir);
    void writeExif();
};

#endif // EXIFWRITER_H
