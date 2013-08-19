##
#   This file is part of Rocs.
#   Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>
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
    ${CMAKE_CURRENT_BINARY_DIR}
)

# basic functionality tests of data structure
set (TestGraphStructure_SRCS
    TestGraphStructure.cpp
)
kde4_add_executable(TestGraphStructure ${TestGraphStructure_SRCS} )
target_link_libraries( TestGraphStructure
    rocscore
    rocs_GraphStructure
    ${KDE4_KROSSUI_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
)
add_test(TestGraphStructure TestGraphStructure)


# graph algorithms
set (TestGraphStructureAlgorithms_SRCS
    TestGraphStructureAlgorithms.cpp
)
kde4_add_executable(TestGraphStructureAlgorithms ${TestGraphStructureAlgorithms_SRCS} )
target_link_libraries( TestGraphStructureAlgorithms
    rocscore
    rocs_GraphStructure
    ${KDE4_KROSSUI_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
)
add_test(TestGraphStructureAlgorithms TestGraphStructureAlgorithms)
