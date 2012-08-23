set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

file(COPY example.gml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(TestGmlFileFormatPlugin_SRCS
    GmlFileFormatTest.cpp
    ../GmlGrammar.cpp
    ../GmlGraphParsingHelper.cpp
    ../GmlFileFormatPlugin.cpp
)
kde4_add_unit_test(TestGmlFileFormatPlugin ${TestGmlFileFormatPlugin_SRCS})
target_link_libraries(TestGmlFileFormatPlugin
    rocslib
    rocs_GraphStructure
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
)
add_test(TestGmlFileFormatPlugin TestGmlFileFormatPlugin)
