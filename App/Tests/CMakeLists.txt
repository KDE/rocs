##
#   This file is part of Rocs.
#   Copyright 2009       Tomaz Canabrava <tomaz.canabrava@gmail.com>
#   Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
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

# Include Manager
set(TestIncludeManager_SRCS TestIncludeManager.cpp)
set(rocs_KCFGS ../settings.kcfgc)
kde4_add_kcfg_files(rocs_Settings_SRCS_TEST ${rocs_KCFGS} )
add_executable(TestIncludeManager ${TestIncludeManager_SRCS} ${rocs_Settings_SRCS_TEST})
add_test(TestIncludeManager TestIncludeManager)
ecm_mark_as_test(TestIncludeManager)
target_link_libraries(TestIncludeManager
    rocscore
    Qt5::Test
)
