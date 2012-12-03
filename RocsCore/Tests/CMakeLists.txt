##
#   This file is part of Rocs.
#   Copyright 2009       Tomaz Canabrava <tomaz.canabrava@gmail.com>
#   Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
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
    ../
    ${CMAKE_CURRENT_BINARY_DIR}}
)

# basic functionality tests of data structure
set (TestDataStructure_SRCS
    ../../RocsCore/Tests/TestDataStructure.cpp
)
kde4_add_executable(TestDataStructure ${TestDataStructure_SRCS} )
target_link_libraries( TestDataStructure
    rocscore
    ${KDE4_KROSSUI_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
)
add_test(TestDataStructure TestDataStructure)


# DynamicPropertiesList
set( testDynamicProperties_SRCS
    ../../RocsCore/Tests/TestDynamicProperties.cpp
)
kde4_add_executable(TestDynamicProperties ${testDynamicProperties_SRCS})
target_link_libraries( TestDynamicProperties
    rocscore
    ${KDE4_KROSSUI_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
)
add_test(TestDynamicProperties TestDynamicProperties)


# loading and saving tests
set (TestLoadSave_SRCS
    ../../RocsCore/Tests/TestLoadSave.cpp
)
file(COPY TestGraphFiles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
kde4_add_executable(TestLoadSave ${TestLoadSave_SRCS} )
target_link_libraries( TestLoadSave
    rocscore
    ${KDE4_KROSSUI_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
)
add_test(TestLoadSave TestLoadSave)


# test data structure plugin interface
set( test_Plugins_SRC ../../RocsCore/Tests/TestPlugins.cpp)
kde4_add_executable( TestPlugins ${test_Plugins_SRC})
target_link_libraries( TestPlugins
    rocscore
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
    ${KDE4_KIO_LIBS}
)
add_test(TestPlugins TestPlugins)


# test script engine
set( test_ScriptEngine_SRC ../../RocsCore/Tests/TestScriptEngine.cpp)
kde4_add_executable( TestScriptEngine ${test_ScriptEngine_SRC})
target_link_libraries( TestScriptEngine
    rocscore
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
    ${KDE4_KIO_LIBS}
)
add_test(TestScriptEngine TestScriptEngine)
