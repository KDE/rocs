##
#   This file is part of Rocs.
#   Copyright 2011       Tomaz Canabrava <tomaz.canabrava@gmail.com>
#   Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>
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
project(LinkedListStructure)

include_directories(
    ${ROCSCORE_INCLUDES}
    ${ROCSVISUALEDITOR_INCLUDES}
)

set(ListStructure_SRC
    ListNode.cpp
    ListStructure.cpp
    ListPlugin.cpp
    NodeListItem.cpp
    LinkedListPointerItem.cpp
)

add_library(rocs_ListStructure MODULE ${ListStructure_SRC})

target_link_libraries(rocs_ListStructure
    ${QT_QTSCRIPT_LIBRARY}
    rocscore
    rocsvisualeditor
)

install(TARGETS rocs_ListStructure  DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES  rocs_ListStructure.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES ListStructure.xml DESTINATION ${DATA_INSTALL_DIR}/rocs/engineapi)
install(FILES ListNode.xml DESTINATION ${DATA_INSTALL_DIR}/rocs/engineapi)

if(KDE4_BUILD_TESTS)
    enable_testing()
    ecm_optional_add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)
