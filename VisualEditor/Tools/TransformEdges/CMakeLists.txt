##
#   This file is part of Rocs.
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

set(TransformEdgesToolsPlugin_SRCS
    TransformEdgesToolsPlugin.cpp
    TransformEdgesWidget.cpp
)

qt5_wrap_ui(TransformEdgesToolsPlugin_SRCS TransformEdgesWidget.ui)
add_library(rocs_transformedgesplugin MODULE ${TransformEdgesToolsPlugin_SRCS})

target_link_libraries(
    rocs_transformedgesplugin
    rocscore
    rocsvisualeditor
)

install(TARGETS rocs_transformedgesplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES rocs_transformedgesplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
