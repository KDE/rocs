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

include_directories(
    ${ROCSCORE_INCLUDES}
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

set(TestTgfFileFormatPlugin_SRCS
    TestTgfFileFormatPlugin.cpp
    ../TgfFileFormatPlugin.cpp
)

kde4_add_unit_test(TestTgfFileFormatPlugin ${TestTgfFileFormatPlugin_SRCS})
target_link_libraries(TestTgfFileFormatPlugin
    rocscore
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
)
add_test(TestTgfFileFormatPlugin TestTgfFileFormatPlugin)
