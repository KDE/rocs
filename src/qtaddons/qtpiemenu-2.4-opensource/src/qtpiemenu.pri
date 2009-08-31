include(../common.pri)
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

qtpiemenu-uselib:!qtpiemenu-buildlib {
    LIBS += -L$$QTPIEMENU_LIBDIR -l$$QTPIEMENU_LIBNAME
} else {
    SOURCES += $$PWD/qtpiemenu.cpp $$PWD/qtpieaction.cpp $$PWD/qtpieitem.cpp
    HEADERS += $$PWD/qtpiemenu.h $$PWD/qtpieaction.h $$PWD/qtpieitem.h
}

win32 {
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QTPIEMENU_EXPORT
    else:qtpiemenu-uselib:DEFINES += QT_QTPIEMENU_IMPORT
}
