set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

set(TestDotFileFormatPlugin_SRCS
    DotFileFormatTest.cpp
    ../DotGrammar.cpp
    ../DotGraphParsingHelper.cpp
    ../DotFileFormatPlugin.cpp
)
kde4_add_unit_test(TestDotFileFormatPlugin ${TestDotFileFormatPlugin_SRCS})
target_link_libraries(TestDotFileFormatPlugin
    rocslib
    rocs_GraphStructure
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
)
add_test(TestDotFileFromatPlugin TestDotFileFromatPlugin)
