set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

set(TestTikzFileFormatPlugin_SRCS
    TestTikzFileFormatPlugin.cpp
    ../TikzFileFormatPlugin.cpp
)

kde4_add_unit_test(TestTikzFileFormatPlugin ${TestTikzFileFormatPlugin_SRCS})
target_link_libraries(TestTikzFileFormatPlugin
    rocslib
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
)
add_test(TestTikzFileFormatPlugin TestTikzFileFormatPlugin)

