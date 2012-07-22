set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

set(TestDotFileFromatPlugin_SRCS
    ParseDotTest.cpp
    ../DotGrammar.cpp
    ../DotGraphParsingHelper.cpp
    ../DotFileFormatPlugin.cpp
)
kde4_add_unit_test(TestDotFileFromatPlugin ${TestDotFileFromatPlugin_SRCS})
target_link_libraries(TestDotFileFromatPlugin
    rocslib
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
)
add_test(TestDotFileFromatPlugin TestDotFileFromatPlugin)
