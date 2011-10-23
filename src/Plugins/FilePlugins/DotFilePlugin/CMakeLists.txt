find_package(Boost 1.43 REQUIRED COMPONENTS graph)

if(Boost_FOUND AND Boost_MINOR_VERSION GREATER 42)
  include_directories(
  ${Boost_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ../../
  )

  ADD_DEFINITIONS(${QT_DEFINITIONS})
  ADD_DEFINITIONS(-DQT_PLUGIN)
  ADD_DEFINITIONS(-DQT_NO_DEBUG)
  ADD_DEFINITIONS(-DQT_SHARED)

  set(dotFilePlugin_SRCS DotFilePlugin.cpp)

  qt4_automoc(${dotFilePlugin_SRCS})
  kde4_add_plugin(rocs_dotFilePlugin ${dotFilePlugin_SRCS})
  add_definitions(-DQT_STL)
  target_link_libraries(rocs_dotFilePlugin
    ${KDE4_KDEUI_LIBS}
    ${Boost_GRAPH_LIBRARY} 
    rocslib
  )

  install(TARGETS rocs_dotFilePlugin  DESTINATION ${PLUGIN_INSTALL_DIR})

  install(FILES rocs_dotFilePlugin.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
endif(Boost_FOUND AND Boost_MINOR_VERSION GREATER 42)

add_subdirectory(Tests)