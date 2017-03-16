QLogParser
---

QLogParser2 improved versionon of QLogParser (https://gitlab.com/FlyTech/QLogParser.git)
difrences:
* New GUI
* Changed parsing mechanism
* Implement geotaging files into exif (QLogParser copies whole picture directory 
                                    to prevent corupting time of modification)
* Implement creating text backup of photo directory (names and time of modification)
* Change source files of RTK events (RTK lib tools creates log with taged timemarks
									it si no longer nescessary to interpolate coordinates)
* Simplify manual modification (No longer need to re run whole process)
* Corelate fileset with timemark using cross corelation (QLogParser tries to fit sets automaticly
									even if sets don't fit)
---
Dependencies:
libz
libexiv2
Qt (built and tested on Qt 5.8)

built and tested on Windows 7 and Windows 10 using MinGW 5.3 
