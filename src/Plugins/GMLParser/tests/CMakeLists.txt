
###### Parser
set (ParseGMLTest_SRC ParseDotTest.cpp ../GMLGrammar.cpp ../GMLGraphParsingHelper.cpp ../GMLParser.cpp)

kde4_add_executable(ParseGMLTest ${ParseGMLTest_SRC})
target_link_libraries( ParseGMLTest
  ${KDE4_KDEUI_LIBS}
  ${QT_QTTEST_LIBRARY}
  rocslib
)
add_test(ParseGMLTest ParseGMLTest)
