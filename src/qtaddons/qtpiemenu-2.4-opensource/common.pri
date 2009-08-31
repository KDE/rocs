infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtpiemenu-uselib
TEMPLATE += fakelib
QTPIEMENU_LIBNAME = $$qtLibraryTarget(QtSolutions_PieMenu-2.4)
TEMPLATE -= fakelib
QTPIEMENU_LIBDIR = $$PWD/lib
unix:qtpiemenu-uselib:!qtpiemenu-buildlib:QMAKE_RPATHDIR += $$QTPIEMENU_LIBDIR
