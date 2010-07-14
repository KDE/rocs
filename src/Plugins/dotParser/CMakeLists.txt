find_package(Boost 1.36 REQUIRED)

include_directories(
 ${Boost_INCLUDE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
 ../../
)

if (KDE4_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif(KDE4_BUILD_TESTS)


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

set(dotParser_SRCS DotGraphParsingHelper.cpp dotgrammar.cpp DotParser.cpp)

qt4_automoc(${dotParser_SRCS})
kde4_add_plugin(rocs_dotParser ${dotParser_SRCS})

target_link_libraries(rocs_dotParser
  ${KDE4_KDEUI_LIBS}
  rocslib
)

install(TARGETS rocs_dotParser  DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES rocs_dotParser.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
