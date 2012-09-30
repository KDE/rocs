##
#   This file is part of Rocs.
#   Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
#   Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
#   Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>
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

project(Rocs)

cmake_minimum_required(VERSION 2.6.0)
#### Boost ####
# In CMake >= 2.8.6, FindBoost.cmake tries to find BoostConfig.cmake which is
# not compatible with CMake's FindBoost. Disable this function.
set (Boost_NO_BOOST_CMAKE TRUE)
find_package(Boost "1.43" REQUIRED)

set(KDE_MIN_VERSION "4.3")
find_package(KDE4 REQUIRED)

include(KDE4Defaults)
remove_definitions(-DQT_NO_STL)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

# Rocs include dirs
set (ROCSCORE_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/RocsCore/"
    "${CMAKE_CURRENT_SOURCE_DIR}/RocsCore/LoadSave/"
)

set (ROCSVISUALEDITOR_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/VisualEditor/"
)

if(${CMAKE_BUILD_TYPE} MATCHES coverage)
    set (CMAKE_CXX_COMPILER cov++)
endif()

include_directories(
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
    ${Boost_INCLUDE_DIRS}
)



# Allow disabling building of optional graph file backands
option(ROCS_BUILD_LOADSAVE "ROCS_BUILD_LOADSAVE" ON)
macro_log_feature(
    ROCS_BUILD_LOADSAVE
    "Graph File Plugins"
    "Building of extra graph file plugins is disabled."
    "edu.kde.org/rocs"
    FALSE
    )

# feature log
macro_log_feature(Boost_FOUND "Boost C++ Libraries" "Required to build Rocs" "http://www.boost.org" TRUE "1.43" "")
if(CMAKE_SOURCE_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    macro_display_feature_log()
endif(CMAKE_SOURCE_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(RocsCore)
add_subdirectory(VisualEditor)
add_subdirectory(App)
add_subdirectory(graphics)
add_subdirectory(doc)
