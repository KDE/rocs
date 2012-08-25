##
#   This file is part of Rocs.
#   Copyright 2011  Andreas Cord-Landwehr <cola@uni-paderborn.de>
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

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

set(AssignValuesToolsPlugin_SRC assignvaluestoolsplugin.cpp assignvalueswidget.cpp)

kde4_add_ui_files( AssignValuesToolsPlugin_SRC assignvalueswidget.ui )


kde4_add_plugin(rocs_assignvaluesplugin ${AssignValuesToolsPlugin_SRC} )
target_link_libraries( rocs_assignvaluesplugin
    ${KDE4_KDEUI_LIBS}
    rocslib)

install( TARGETS rocs_assignvaluesplugin  DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES rocs_assignvaluesplugin.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
