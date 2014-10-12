##
#   This file is part of Rocs.
#   Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
#   Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
#   Copyright 2011-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License as
#   published by the Free Software Foundation; either version 2 of
#   the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

project(rocs)
cmake_minimum_required(VERSION 3.0.0)

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
find_package(KF5DocTools)
find_package(Boost "1.49" REQUIRED)
# find_package(Grantlee REQUIRED) #FIXME deactivate for now

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(ECMAddTests)
include(ECMInstallIcons)
include(ECMOptionalAddSubdirectory)
include(ECMSetupVersion)
include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)
include(FeatureSummary)

find_package(Qt5 5.3 REQUIRED NO_MODULE COMPONENTS
  Concurrent
  Core
  Gui
  QuickWidgets
  Script
  WebKit
  WebKitWidgets
  Widgets
  ScriptTools
  Svg
  Test
  XmlPatterns
)
find_package(KF5 REQUIRED COMPONENTS
  Config
  CoreAddons
  Declarative
  DocTools
  I18n
  ItemViews
  TextEditor
  XmlGui
  NewStuff
  ItemViews
  Service
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
#     ${Grantlee_INCLUDE_DIRS}
)

remove_definitions(-DQT_NO_CAST_TO_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)
remove_definitions(-DQT_NO_KEYWORDS)

ecm_optional_add_subdirectory(libgraphtheory)
ecm_optional_add_subdirectory(src)
ecm_optional_add_subdirectory(icons)
ecm_optional_add_subdirectory(doc)

set_package_info(Boost "Boost C++ Libraries" "http://www.boost.org")
feature_summary(WHAT ALL)

install(FILES rocs.appdata.xml DESTINATION ${SHARE_INSTALL_PREFIX}/appdata/)
