include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../Core )

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
kde4_add_executable(TestIncludeManager ${test_IncludeManager_SRC})
target_link_libraries( TestIncludeManager
# ${KDE4_KROSSUI_LIBS}
${QT_QTTEST_LIBRARY}
${KDE4_KDECORE_LIBS}
${QT_QTSCRIPT_LIBRARY}
)
add_test(TestIncludeManager TestIncludeManager)



