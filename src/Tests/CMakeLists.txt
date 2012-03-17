include_directories(
#     ${KDE4_INCLUDES} ${QT_INCLUDES}
#     ${CMAKE_BINARY_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}}
#     ${CMAKE_CURRENT_BINARY_DIR}}/..
    )

# basic functionality tests of data structure
set (TestDataStructure_SRCS
    TestDataStructure.cpp
)
kde4_add_executable(TestDataStructure ${TestDataStructure_SRCS} )
target_link_libraries( TestDataStructure
    rocslib
    ${KDE4_KROSSUI_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
)
add_test(TestDataStructure TestDataStructure)


# DynamicPropertiesList
set( testDynamicProperties_SRCS
    TestDynamicProperties.cpp
)
kde4_add_executable(TestDynamicProperties ${testDynamicProperties_SRCS})
target_link_libraries( TestDynamicProperties
    rocslib
    ${KDE4_KROSSUI_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
)
add_test(TestDynamicProperties TestDynamicProperties)


# Include Manager
set( test_IncludeManager_SRC TestIncludeManager.cpp ../Scripts/IncludeManager.cpp)
set(rocs_KCFGS ../settings.kcfgc)
kde4_add_kcfg_files(rocs_Settings_SRCS_TEST ${rocs_KCFGS} )
kde4_add_executable( TestIncludeManager ${test_IncludeManager_SRC} ${rocs_Settings_SRCS_TEST})
target_link_libraries( TestIncludeManager
    # ${KDE4_KROSSUI_LIBS}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
    ${KDE4_KIO_LIBS}
)
add_test(TestIncludeManager TestIncludeManager)


# loading and saving tests
set (TestLoadSave_SRCS
    TestLoadSave.cpp
)
kde4_add_executable(TestLoadSave ${TestLoadSave_SRCS} )
target_link_libraries( TestLoadSave
    rocslib
    ${KDE4_KROSSUI_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
)
add_test(TestLoadSave TestLoadSave)


#Data structure Plugins
set( test_Plugins_SRC TestPlugins.cpp)
kde4_add_executable( TestPlugins ${test_Plugins_SRC})
target_link_libraries( TestPlugins
        rocslib
        ${QT_QTTEST_LIBRARY}
        ${KDE4_KDECORE_LIBS}
        ${QT_QTSCRIPT_LIBRARY}
        ${KDE4_KIO_LIBS}
)
add_test(TestPlugins TestPlugins)
