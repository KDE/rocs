TEMPLATE=lib
CONFIG += qt dll qtpiemenu-buildlib
mac:CONFIG += absolute_library_soname
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
include(../src/qtpiemenu.pri)
TARGET = $$QTPIEMENU_LIBNAME
DESTDIR = $$QTPIEMENU_LIBDIR
win32 {
    DLLDESTDIR = $$[QT_INSTALL_BINS]
    QMAKE_DISTCLEAN += $$[QT_INSTALL_BINS]\\$${QTPIEMENU_LIBNAME}.dll
}
target.path = $$DESTDIR
INSTALLS += target
