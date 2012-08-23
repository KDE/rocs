
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

set(TestTikzFileFromatPlugin_SRCS
    TestTikzFileFormatPlugin.cpp
    ../TikzFileFormatPlugin.cpp
)

kde4_add_unit_test(TestTikzFileFromatPlugin ${TestTikzFileFromatPlugin_SRCS})
target_link_libraries(TestTikzFileFromatPlugin
    rocslib
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
)
add_test(TestTikzFileFromatPlugin TestTikzFileFromatPlugin)

