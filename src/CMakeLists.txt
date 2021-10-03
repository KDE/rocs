##
#   This file is part of Rocs.
#   SPDX-FileCopyrightText: 2008-2011 Tomaz Canabrava <tomaz.canabrava@gmail.com>
#   SPDX-FileCopyrightText: 2010-2012 Wagner Reck <wagner.reck@gmail.com>
#   SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
#   SPDX-License-Identifier: GPL-2.0-or-later
##

add_definitions(-DTRANSLATION_DOMAIN=\"rocs\")

ecm_setup_version(${RELEASE_SERVICE_VERSION}
    VARIABLE_PREFIX ROCS
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/rocsversion.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/RocsConfigVersion.cmake"
)

ecm_optional_add_subdirectory(plugins)

include_directories(
    ./
)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(rocs)

target_sources(rocs PRIVATE
    ui/journalwidget.cpp
    ui/sidedockwidget.cpp
    ui/fileformatdialog.cpp
    ui/mainwindow.cpp
    ui/codeeditorwidget.cpp
    ui/documenttypeswidget.cpp
    ui/grapheditorwidget.cpp
    ui/nodetypesdelegate.cpp
    ui/edgetypesdelegate.cpp
    ui/scriptoutputwidget.cpp
    project/project.cpp
    plugins/scriptapi/scriptapiwidget.cpp
    plugins/scriptapi/object.cpp
    plugins/scriptapi/property.cpp
    plugins/scriptapi/method.cpp
    plugins/scriptapi/parameter.cpp
    plugins/scriptapi/scriptapimanager.cpp
    plugins/scriptapi/scriptapimodel.cpp
    main.cpp
)

ki18n_wrap_ui(rocs
    ui/journalwidget.ui
    ui/scriptoutputwidget.ui
    plugins/scriptapi/scriptapiwidget.ui
)

kconfig_add_kcfg_files(rocs settings.kcfgc)

# add icon to executable on windows and mac
ecm_add_app_icon(rocs
    ICONS
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/16-apps-rocs.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/22-apps-rocs.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/32-apps-rocs.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/48-apps-rocs.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/64-apps-rocs.png
    ${CMAKE_CURRENT_SOURCE_DIR}/../icons/128-apps-rocs.png
)

target_link_libraries(rocs
    PUBLIC
        rocsgraphtheory
        Grantlee5::Templates
        KF5::Archive
        KF5::CoreAddons
        KF5::Crash
        KF5::I18n
        KF5::ItemViews
        KF5::Parts
        KF5::TextEditor
        Qt5::Core
        Qt5::Quick
        Qt5::QuickWidgets
        Qt5::XmlPatterns
)

################## INSTALLS ##########################
install(TARGETS rocs ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.rocs.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES rocsui.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/rocs)
install(FILES rocs.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})

ecm_optional_add_subdirectory(plugins/scriptapi)
ecm_optional_add_subdirectory(autotests)
