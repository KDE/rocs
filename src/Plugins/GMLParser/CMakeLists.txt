find_package(Boost 1.36 REQUIRED)

include_directories(
 ${Boost_INCLUDE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
 ../../
)

if (KDE4_BUILD_TESTS)
#   enable_testing()
#   add_subdirectory(tests)
endif(KDE4_BUILD_TESTS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

set(GMLParser_SRCS GMLGraphParsingHelper.cpp GMLGrammar.cpp GMLParser.cpp)

qt4_automoc(${GMLParser_SRCS})
kde4_add_plugin(rocs_GMLParser ${GMLParser_SRCS})

target_link_libraries(rocs_GMLParser
  ${KDE4_KDEUI_LIBS}
  rocslib
)

install(TARGETS rocs_GMLParser  DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES rocs_GMLParser.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
