find_package(KDE4 REQUIRED)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include(KDE4Defaults)

add_subdirectory(src)

include_directories( ${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )



########### install files ###############






#original Makefile.am contents follow:
#SUBDIRS = src

#EXTRA_DIST = TODO README AUTHORS Doxyfile INSTALL ChangeLog COPYING

#AUTOMAKE_OPTIONS = foreign
