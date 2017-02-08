#include "timeutils.h"

//GPS standard (time in NMEA)
#define MS_PER_SEC      1000ul
#define MS_PER_MIN      (60*MS_PER_SEC)
#define MS_PER_HOUR     (60*MS_PER_MIN)
#define MS_PER_DAY      (24*MS_PER_HOUR)

#define SEC_PER_MIN     60
#define SEC_PER_HOUR    (60*SEC_PER_MIN)

//Ashtech
#define BCD_PER_MSEC	1000ul
#define BCD_PER_SEC		100000ul
#define BCD_PER_MIN		10000000ul
#define BCD_PER_HOUR	1000000000ul

#define BCD_SHORT_PER_SEC	100ul
#define BCD_SHORT_PER_MIN	10000ul
#define BCD_SHORT_PER_HOUR	1000000ul

#define DAY_IN_UNIX_TIME		86400UL		//seconds
#define WEEK_IN_UNIX_TIME		604800UL 	//seconds
#define UNIX_2_GPS_TIME_OFFS	0			//16UL 		//valid for 2013

timeUtils::timeUtils()
{
}

unsigned long
timeUtils::convTimeAshTech2GPS(unsigned long in)
{
    long msec = in % BCD_PER_MSEC;
    long sec = (in % BCD_PER_SEC)/BCD_PER_MSEC;
    long min = (in % BCD_PER_MIN)/BCD_PER_SEC;
    long hr  = (in % BCD_PER_HOUR)/BCD_PER_MIN;
    return hr * MS_PER_HOUR + min * MS_PER_MIN + sec * MS_PER_SEC + msec;
}

unsigned long
timeUtils::convTimeAshShort2GPS(unsigned long in)
{
    long sec = in % BCD_SHORT_PER_SEC;
    long min = (in % BCD_SHORT_PER_MIN)/BCD_SHORT_PER_SEC;
    long hr  = (in % BCD_SHORT_PER_HOUR)/BCD_SHORT_PER_MIN;
    return hr * SEC_PER_HOUR + min * SEC_PER_MIN + sec;
}

// Input: Unix time (Unix epoch started on Thursday)
// Output: GPS for current week (for GPS Sunday is 0)
unsigned long
timeUtils::convTimeUnix2Ardu(time_t in) //Unix(UTC) to Ardu(GPS for current week)
{
    unsigned long out;

    out = (unsigned long)in % DAY_IN_UNIX_TIME;

    out += UNIX_2_GPS_TIME_OFFS;
    out *= 1000;

    return out;
}

void timeUtils::CreatePictureBackupFile( QDir sourceDir, long cam2utcClkOffs, QString fileToSave)
{
    QFile file(fileToSave);
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    if ( sourceDir.exists())
    {

        //Do NOT use for when iterating over a directory with Qt
        QDirIterator dir_iter(sourceDir);
        while(dir_iter.hasNext())
        {
            dir_iter.next();
            QFileInfo fInfo(dir_iter.filePath());
            if ( fInfo.isFile() )
            {
                QDateTime	picTimeCAM(fInfo.lastModified()); // picture time as measured by the camera
                QDateTime	picTimeUTC(picTimeCAM);           // picture time UTC
                picTimeUTC.addSecs(cam2utcClkOffs);

                uint picTimeArdu(convTimeUnix2Ardu(picTimeUTC.toTime_t()));	// picture time as stored by ArduPilot 2.5
                stream << dir_iter.fileName() << ';' << QString::number( picTimeArdu) << '\n';
                //insert method allows for working both on ordered and unordered sets
            }
        }
    }
}


void
timeUtils::showTime(unsigned long time, std::stringstream &ss)
{
    using namespace std;
    int hour, min, sec, msec;
    hour	= (int) ((time%MS_PER_DAY)/MS_PER_HOUR);
    min		= (int) ((time%MS_PER_DAY%MS_PER_HOUR)/MS_PER_MIN);
    sec		= (int) ((time%MS_PER_DAY%MS_PER_HOUR%MS_PER_MIN)/MS_PER_SEC);
    msec	= (int) (time%MS_PER_DAY%MS_PER_HOUR%MS_PER_MIN%MS_PER_SEC);
    ss.str("");
    ss.clear();
    ss.width(2);
    ss.fill('0');
    ss <<hour<<':';
    ss.width(2);
    ss.fill('0');
    ss <<min<<':';
    ss.width(2);
    ss.fill('0');
    ss <<sec<<'.';
    ss.width(3);
    ss.fill('0');
    ss <<msec<<"\t";
}

void timeUtils::showTime(unsigned long time, QString *s)
{
    int hour, min, sec, msec;

    hour	= (int) ((time%MS_PER_DAY)/MS_PER_HOUR);
    min		= (int) ((time%MS_PER_DAY%MS_PER_HOUR)/MS_PER_MIN);
    sec		= (int) ((time%MS_PER_DAY%MS_PER_HOUR%MS_PER_MIN)/MS_PER_SEC);
    msec	= (int) (time%MS_PER_DAY%MS_PER_HOUR%MS_PER_MIN%MS_PER_SEC);
    *s = QString::number(hour) + ":" + QString::number(min) + ":" +  QString::number(sec) + "." + QString::number(msec/100)+ QString::number(msec%100/10)+ QString::number(msec%10);
}

unsigned long timeUtils::convTimeUTCToGPS(QString y, QString m, QString d, QString h, QString mt, QString s)
{
    QDateTime t = QDateTime::fromString(y+m+d+h+mt, "yyyyMdHm");
    int day = t.date().dayOfWeek();
    if (day == 7) day = 0;
    return day * 86400000 + t.time().hour() * 3600000+ t.time().minute() * 60000 + s.toDouble()*1000;
}
