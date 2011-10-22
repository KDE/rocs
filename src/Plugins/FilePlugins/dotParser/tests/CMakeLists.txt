
###### Parser
set (ParseDotTest_SRC ParseDotTest.cpp ../dotgrammar.cpp ../DotGraphParsingHelper.cpp ../DotParser.cpp)

kde4_add_executable(ParseDotTest ${ParseDotTest_SRC})
target_link_libraries( ParseDotTest
  ${KDE4_KDEUI_LIBS}
  ${QT_QTTEST_LIBRARY}
  rocslib
)
add_test(ParseDotTest ParseDotTest)
