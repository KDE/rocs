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

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)


set(ListStructure_SRC
    ListNode.cpp
    ListStructure.cpp
    ListPlugin.cpp
    NodeListItem.cpp
    LinkedListPointerItem.cpp
)

kde4_add_plugin(rocs_ListStructure ${ListStructure_SRC} )
kde4_add_library(rocs_ListStructure SHARED ${ListStructure_SRC} )

target_link_libraries(rocs_ListStructure
                ${KDE4_KDEUI_LIBS}
                ${QT_QTSCRIPT_LIBRARY}
                ${QT_QTSCRIPTTOOLS_LIBRARY}
                rocslib)

install(TARGETS rocs_ListStructure  DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES  rocs_ListStructure.desktop  DESTINATION ${SERVICES_INSTALL_DIR})

if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory( Tests )
endif(KDE4_BUILD_TESTS)
