include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../Core )

set (testGraphs_SRCS  
   testGraph.cpp
)

kde4_add_executable(testGraphs ${testGraphs_SRCS} )

target_link_libraries( testGraphs
  librocscore
  ${KDE4_KROSSUI_LIBS}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTTEST_LIBRARY}
  ${KDE4_KDECORE_LIBS}
  ${QT_QTSCRIPT_LIBRARY}
) 

kde4_add_executable(TestDinamicProperties testdinamicproperties.cpp)
target_link_libraries( TestDinamicProperties
  librocscore
  ${KDE4_KROSSUI_LIBS}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTTEST_LIBRARY}
  ${KDE4_KDECORE_LIBS}
  ${QT_QTSCRIPT_LIBRARY}
) 

add_test(TestDinamicProperties TestDinamicProperties)
add_test(TestGraphs testGraphs)
