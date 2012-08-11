
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

set(TestTgfFileFromatPlugin_SRCS
    TestTgfFileFormatPlugin.cpp
    ../TgfFileFormatPlugin.cpp
)

kde4_add_unit_test(TestTgfFileFromatPlugin ${TestTgfFileFromatPlugin_SRCS})
target_link_libraries(TestTgfFileFromatPlugin
    rocslib
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
)
add_test(TestTgfFileFromatPlugin TestTgfFileFromatPlugin)
