
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

set(TestTgfFileFormatPlugin_SRCS
    TestTgfFileFormatPlugin.cpp
    ../TgfFileFormatPlugin.cpp
)

kde4_add_unit_test(TestTgfFileFormatPlugin ${TestTgfFileFormatPlugin_SRCS})
target_link_libraries(TestTgfFileFormatPlugin
    rocslib
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
)
add_test(TestTgfFileFormatPlugin TestTgfFileFormatPlugin)
