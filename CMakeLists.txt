# SPDX-FileCopyrightText: 2008-2011 Tomaz Canabrava <tomaz.canabrava@gmail.com>
# SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
# SPDX-FileCopyrightText: 2011-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: GPL-2.0-or-later

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "25")
set(RELEASE_SERVICE_VERSION_MINOR "11")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(rocs LANGUAGES CXX VERSION ${RELEASE_SERVICE_VERSION})

set(QT_MIN_VERSION "6.7.0")
set(KF_MIN_VERSION "6.0.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEClangFormat)

include(ECMAddAppIcon)
include(ECMAddTests)
include(ECMInstallIcons)
include(ECMOptionalAddSubdirectory)
include(ECMSetupVersion)
include(ECMEnableSanitizers)
include(FeatureSummary)
include(GenerateExportHeader)

find_package(Boost "1.49" REQUIRED)

if(BUILD_TESTING)
    set(QT_CONDITIONAL_COMPONENTS Test)
endif()

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS
    Core
    Gui
    QuickWidgets
    Widgets
    Svg
    ${QT_CONDITIONAL_COMPONENTS}
)
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Archive
    Config
    CoreAddons
    Crash
    I18n
    ItemViews
    TextEditor
    TextTemplate
    TextWidgets
    XmlGui
)
find_package(KF6DocTools)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
)

# added by KDE_COMPILERSETTINGS_LEVEL 5.85.0
remove_definitions(-DQT_NO_CAST_FROM_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)

ecm_optional_add_subdirectory(libgraphtheory)
ecm_optional_add_subdirectory(src)
ecm_optional_add_subdirectory(icons)
if(KF5DocTools_FOUND)
    ecm_optional_add_subdirectory(doc)
endif()

ki18n_install(po)
if (KF5DocTools_FOUND)
    kdoctools_install(po)
endif()
set_package_properties(Boost PROPERTIES DESCRIPTION "Boost C++ Libraries" URL "https://www.boost.org")
feature_summary(WHAT ALL)

install(FILES org.kde.rocs.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
