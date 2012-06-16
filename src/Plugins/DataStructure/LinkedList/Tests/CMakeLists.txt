include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}}
)

# basic functionality tests of data structure
set (TestLinkedListStructure_SRCS
    TestLinkedListStructure.cpp
)
kde4_add_executable(TestLinkedListStructure ${TestLinkedListStructure_SRCS} )
target_link_libraries( TestLinkedListStructure
    rocslib
    rocs_ListStructure
    ${KDE4_KROSSUI_LIBS}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
)
add_test(TestLinkedListStructure TestLinkedListStructure)
