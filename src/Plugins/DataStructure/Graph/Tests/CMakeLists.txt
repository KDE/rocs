include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}}
)

# basic functionality tests of data structure
set (TestGraphStructure_SRCS
    TestGraphStructure.cpp
)
kde4_add_executable(TestGraphStructure ${TestGraphStructure_SRCS} )
target_link_libraries( TestGraphStructure
    rocslib
    rocs_GraphStructure
    ${KDE4_KROSSUI_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
)
add_test(TestGraphStructure TestGraphStructure)
