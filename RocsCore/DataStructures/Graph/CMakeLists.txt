##
#   This file is part of Rocs.
#   Copyright 2011       Tomaz Canabrava <tomaz.canabrava@gmail.com>
#   Copyright 2011       Wagner Reck <wagner.reck@gmail.com>
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

project(GraphStructure)

include_directories(
    ${ROCSCORE_INCLUDES}
    ${ROCSVISUALEDITOR_INCLUDES}
)

set(GraphStructure_SRC
    GraphNode.cpp
    GraphStructure.cpp
    GraphPlugin.cpp
    NodeItem.cpp
    EdgeItem.cpp)

add_library(rocs_GraphStructure SHARED ${GraphStructure_SRC})

target_link_libraries(rocs_GraphStructure
    ${QT_QTSCRIPT_LIBRARY}
    rocscore
    rocsvisualeditor
)

install(TARGETS rocs_GraphStructure  DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES rocs_GraphStructure.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES GraphStructure.xml DESTINATION ${DATA_INSTALL_DIR}/rocs/engineapi)
install(FILES GraphNode.xml DESTINATION ${DATA_INSTALL_DIR}/rocs/engineapi)
install(FILES GraphEdge.xml DESTINATION ${DATA_INSTALL_DIR}/rocs/engineapi)

if(KDE4_BUILD_TESTS)
    enable_testing()
    ecm_optional_add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)
