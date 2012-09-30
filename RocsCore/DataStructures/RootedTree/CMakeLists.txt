##
#   This file is part of Rocs.
#   Copyright 2012       Wagner Reck <wagner.reck@gmail.com>
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

include_directories(
    ${ROCSCORE_INCLUDES}
    ${ROCSVISUALEDITOR_INCLUDES}
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

set(rocs_RootedTree_SRCS
    RootedTreeNode.cpp
    RootedTreeStructure.cpp
    RootedTreePlugin.cpp
    RootedTreeNodeItem.cpp
    RootedTreeEdgeItem.cpp
)

kde4_add_plugin(rocs_RootedTreeStructure ${rocs_RootedTree_SRCS})

target_link_libraries(rocs_RootedTreeStructure
                ${KDE4_KDEUI_LIBS}
                ${QT_QTSCRIPT_LIBRARY}
                ${QT_QTSCRIPTTOOLS_LIBRARY}
                rocscore
                rocsvisualeditor)

install(TARGETS rocs_RootedTreeStructure DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES RootedTreeui.rc DESTINATION ${DATA_INSTALL_DIR}/rocs_rootedtree)
install(FILES rocs_RootedTreeStructure.desktop DESTINATION ${SERVICES_INSTALL_DIR})
