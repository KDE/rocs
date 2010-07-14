
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(rocs_KMLParserTest_SRCS rocs_KMLParserTest.cpp)
qt4_automoc(${rocs_KMLParserTest_SRCS})
add_executable(rocs_KMLParserTest ${rocs_KMLParserTest_SRCS})
add_test(rocs_KMLParserTest rocs_KMLParserTest)
target_link_libraries(rocs_KMLParserTest ${QT_QTTEST_LIBRARY})
