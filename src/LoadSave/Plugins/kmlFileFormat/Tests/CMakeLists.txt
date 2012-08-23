set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

set(TestKmlFileFormatPlugin_SRCS
    KmlFileFormatTest.cpp
    ../KmlHandler.cpp
    ../KmlFileFormatPlugin.cpp
)
kde4_add_unit_test(TestKmlFileFormatPlugin ${TestKmlFileFormatPlugin_SRCS})
target_link_libraries(TestKmlFileFormatPlugin
    rocslib
    rocs_GraphStructure
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
)
add_test(TestKmlFileFormatPlugin TestKmlFileFormatPlugin)
