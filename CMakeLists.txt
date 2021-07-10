##
#   This file is part of Rocs.
#   SPDX-FileCopyrightText: 2008-2011 Tomaz Canabrava <tomaz.canabrava@gmail.com>
#   SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
#   SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
#   SPDX-License-Identifier: GPL-2.0-or-later
##

cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(rocs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "21")
set(RELEASE_SERVICE_VERSION_MINOR "11")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

find_package(ECM 5.40.0 REQUIRED NO_MODULE)
find_package(KF5DocTools)
find_package(Boost "1.49" REQUIRED)
find_package(Grantlee5 "5.0.0" REQUIRED)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(ECMAddAppIcon)
include(ECMAddTests)
include(ECMInstallIcons)
include(ECMOptionalAddSubdirectory)
include(ECMSetupVersion)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(FeatureSummary)
include(GenerateExportHeader)

find_package(Qt5 5.11 REQUIRED NO_MODULE COMPONENTS
    Core
    Gui
    QuickWidgets
    Script
    Widgets
    ScriptTools
    Svg
    Test
    XmlPatterns
)
find_package(KF5 5.17 REQUIRED COMPONENTS
    Archive
    Config
    CoreAddons
    Crash
    I18n
    ItemViews
    TextEditor
    XmlGui
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
)

add_definitions(-DQT_NO_CAST_TO_ASCII)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)
remove_definitions(-DQT_NO_KEYWORDS)

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
