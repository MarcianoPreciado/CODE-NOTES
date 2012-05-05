#!/bin/sh

#echo '#include <iostream>\n#include <vector>' > pchheader.h

clang -x c++-header -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtScript -I/usr/include/qt4 -I/usr/share/qt4/mkspecs/linux-g++-64 ./pchheader.h -fno-exceptions -fnu-runtime -o ./pchheader.pch
