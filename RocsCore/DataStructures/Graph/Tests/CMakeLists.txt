##
#   This file is part of Rocs.
#   Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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
    ../GraphStructure.cpp
    ../GraphNode.cpp
)
add_executable(TestGraphStructure ${TestGraphStructure_SRCS})
add_test(TestGraphStructure TestGraphStructure)
ecm_mark_as_test(TestGraphStructure)
set_target_properties(TestGraphStructure PROPERTIES COMPILE_FLAGS -DROCS_GRAPHSTRUCTURE_EXPORT=)
target_link_libraries( TestGraphStructure
    rocscore
    Qt5::Test
)

# graph algorithms
set (TestGraphStructureAlgorithms_SRCS
    TestGraphStructureAlgorithms.cpp
    ../GraphStructure.cpp
    ../GraphNode.cpp
)
add_executable(TestGraphStructureAlgorithms ${TestGraphStructureAlgorithms_SRCS})
add_test(TestGraphStructureAlgorithms TestGraphStructureAlgorithms)
ecm_mark_as_test(TestGraphStructureAlgorithms)
set_target_properties(TestGraphStructureAlgorithms PROPERTIES COMPILE_FLAGS -DROCS_GRAPHSTRUCTURE_EXPORT=)
target_link_libraries(TestGraphStructureAlgorithms
    rocscore
    Qt5::Test
)
