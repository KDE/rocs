include_directories(
#     ${KDE4_INCLUDES} ${QT_INCLUDES}
#     ${CMAKE_BINARY_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}}
#     ${CMAKE_CURRENT_BINARY_DIR}}/..
    )

set (testGraphs_SRCS
   testGraph.cpp
)
kde4_add_executable(testGraphs ${testGraphs_SRCS} )
target_link_libraries( testGraphs
    rocslib
  ${KDE4_KROSSUI_LIBS}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTTEST_LIBRARY}
  ${KDE4_KDECORE_LIBS}
  ${QT_QTSCRIPT_LIBRARY}
)
add_test(TestGraphs testGraphs)

#DynamicPropertiesList
set( testDinamicProperties_SRCS TestDynamicProperties.cpp)
kde4_add_executable(TestDynamicProperties ${testDinamicProperties_SRCS})
target_link_libraries( TestDynamicProperties
    rocslib
    ${KDE4_KROSSUI_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
)
add_test(TestDynamicProperties TestDynamicProperties)

#Include Manager
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



#Data structure
set( test_DataStructure_SRC TestDataStructure.cpp)



kde4_add_executable( TestDataStructure ${test_DataStructure_SRC})
target_link_libraries( TestDataStructure
        rocslib
        ${QT_QTTEST_LIBRARY}
        ${KDE4_KDECORE_LIBS}
        ${QT_QTSCRIPT_LIBRARY}
        ${KDE4_KIO_LIBS}
)
add_test(TestDataStructure TestDataStructure)
