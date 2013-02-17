##
#   This file is part of Rocs.
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

project(RocsGmlFileFormatPlugin)

include_directories(
    ${ROCSCORE_INCLUDES}
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
)
add_definitions(-DQT_STL)

set(gmlFileFormatPlugin_SRCS
    GmlFileFormatPlugin.cpp
    GmlGrammar.cpp
    GmlGraphParsingHelper.cpp
)

kde4_add_plugin(rocs_gmlfileformat ${gmlFileFormatPlugin_SRCS})

target_link_libraries(rocs_gmlfileformat
    rocscore
    rocs_GraphStructure
    ${KDE4_KDEUI_LIBS}
)

install(TARGETS rocs_gmlfileformat DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES rocs_gmlfileformatplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})

if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)
