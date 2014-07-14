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
    ../
    ${CMAKE_CURRENT_BINARY_DIR}
)

# basic functionality tests of data structure
set (TestDataStructure_SRCS
    ../../RocsCore/Tests/TestDataStructure.cpp
)
add_executable(TestDataStructure ${TestDataStructure_SRCS})
add_test(TestDataStructure TestDataStructure)
ecm_mark_as_test(TestDataStructure)
target_link_libraries(TestDataStructure
    rocscore
    Qt5::Test
)

# DynamicPropertiesList
set(TestDynamicProperties_SRCS
    ../../RocsCore/Tests/TestDynamicProperties.cpp
)
add_executable(TestDynamicProperties ${TestDynamicProperties_SRCS})
add_test(TestDynamicProperties TestDynamicProperties)
ecm_mark_as_test(TestDynamicProperties)
target_link_libraries(TestDynamicProperties
    rocscore
    Qt5::Test
)

# test data structure plugin interface
set(TestPlugins_SRC
    ../../RocsCore/Tests/TestPlugins.cpp
)
add_executable(TestPlugins ${TestPlugins_SRC})
add_test(TestPlugins TestPlugins)
ecm_mark_as_test(TestPlugins)
target_link_libraries(TestPlugins
    rocscore
    Qt5::Test
)

# test script engine
set(TestScriptEngine_SRCS
    ../../RocsCore/Tests/TestScriptEngine.cpp
)
add_executable(TestScriptEngine ${TestScriptEngine_SRCS})
add_test(TestScriptEngine TestScriptEngine)
ecm_mark_as_test(TestScriptEngine)
target_link_libraries(TestScriptEngine
    rocscore
    Qt5::Test
)
