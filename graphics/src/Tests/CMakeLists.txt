include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../Core )

set (testGraphs_SRCS 
  testGraph.cpp
)

kde4_add_executable(testGraphs ${testGraphs_SRCS} )

target_link_libraries( testGraphs
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KROSSUI_LIBS}
  ${QT_QTTEST_LIBRARY}
  ${QT_QTSCRIPT_LIBRARY}
  librocscore
) 
