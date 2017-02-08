#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <ctime>	//asciitime()
#include <sstream>
#include <QString>
#include <QMultiMap>
#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QStringList>

typedef QMultiMap<uint, QString> fileSet_TDef;

class timeUtils
{
public:
    timeUtils();
    static unsigned long convTimeAshTech2GPS(unsigned long in);

    static unsigned long convTimeAshShort2GPS(unsigned long in);

    //Unix(UTC) to Ardu(GPS for current week)
    static unsigned long convTimeUnix2Ardu(time_t);

    //also does conversion from Camera time to GPS time
    /* remember that Unix time is automatically converted to local (summer, winter etc.) */
    static void ReadPictureDir(fileSet_TDef &fileSet, QDir sourceDir, long cam2utcClkOffs);
    static void ReadPictureFile(fileSet_TDef &fileSet, QString path);
    static void CreatePictureBackupFile(QDir sourceDir, long cam2utcClkOffs, QString fileToSave);

    static void showTime(unsigned long time, std::stringstream &ss);
    static void showTime(unsigned long time, QString *s);
    //static std::time_t storeMTime(std::string filename);
    //static void restoreMTime(std::string filename, std::time_t t);
    static unsigned long convTimeUTCToGPS(QString y, QString m, QString d, QString h, QString mt, QString s);

};

#endif // TIMEUTILS_H
