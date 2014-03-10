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
cmake_minimum_required(VERSION 2.8.11)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(ECMInstallIcons)

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)
include(FeatureSummary)

find_package(Qt5 5.2 REQUIRED NO_MODULE COMPONENTS
  Concurrent
  Core
  Gui
  Script
  WebKit
  WebKitWidgets
  Widgets
  ScriptTools
  Svg
  XmlPatterns
)
find_package(KF5 CONFIG REQUIRED COMPONENTS
  Config
  CoreAddons
  DocTools
  I18n
  KDE4Support
  TextEditor
  XmlGui
  NewStuff
  ItemViews
  Service
)

#### Boost ####
# In CMake >= 2.8.6, FindBoost.cmake tries to find BoostConfig.cmake which is
# not compatible with CMake's FindBoost. Disable this function.
set(Boost_NO_BOOST_CMAKE TRUE)
find_package(Boost "1.43" REQUIRED)
set_package_info(Boost "Boost C++ Libraries" "http://www.boost.org")
# find_package(Grantlee REQUIRED) #FIXME deactivate for now

# message(${Qt5Script_INCLUDES})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_INSTALL_PREFIX}/include/KDE #FIXME
    ${Boost_INCLUDE_DIRS}
#     ${Grantlee_INCLUDE_DIRS}
)


remove_definitions(-DQT_NO_CAST_TO_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)
remove_definitions(-DQT_NO_KEYWORDS)
# revisit if still needed to build boost
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")

# Rocs include dirs
set(ROCSCORE_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/RocsCore/"
    "${CMAKE_CURRENT_SOURCE_DIR}/RocsCore/LoadSave/"
)

set(ROCSVISUALEDITOR_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/VisualEditor/"
)

# allow disabling building of optional graph file backends
option(ROCS_BUILD_LOADSAVE "ROCS_BUILD_LOADSAVE" ON)
add_feature_info(OptionalGraphFiles
    ROCS_BUILD_LOADSAVE
    "Building of extra graph file plugins"
)

# enable or disable experimental plugins
# experimental plugins are (by definition) not suited for release builds
option(ROCS_EXPERIMENTAL_PLUGINS "ROCS_EXPERIMENTAL_PLUGINS" OFF)
add_feature_info(ExperimentalPlugins
    ROCS_EXPERIMENTAL_PLUGINS
    "Building experimental/unstable plugins."
)

# main includes
add_subdirectory(RocsCore)
add_subdirectory(VisualEditor)
add_subdirectory(App)

# feature log
if(CMAKE_SOURCE_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    feature_summary(WHAT ALL)
endif(CMAKE_SOURCE_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(graphics)
# add_subdirectory(doc) //FIXME port documentation
