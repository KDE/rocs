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
set (TestLinkedListStructure_SRCS
    TestLinkedListStructure.cpp
    ../ListNode.cpp
    ../ListStructure.cpp
)
add_executable(TestLinkedListStructure ${TestLinkedListStructure_SRCS})
add_test(TestLinkedListStructure TestLinkedListStructure)
ecm_mark_as_test(TestLinkedListStructure)
set_target_properties(TestLinkedListStructure PROPERTIES COMPILE_FLAGS -DROCS_LISTSTRUCTURE_EXPORT=)
target_link_libraries( TestLinkedListStructure
    rocscore
    KF5::CoreAddons
    Qt5::Test
)
