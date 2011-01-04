# # project(rocs_KMLParser)
# #
# # cmake_minimum_required(VERSION 2.6)
# #
# # find_package(Qt4 REQUIRED)
# # find_package(KDE4 REQUIRED)
# # include(KDE4Defaults)
# 
# ADD_DEFINITIONS(${QT_DEFINITIONS})
# ADD_DEFINITIONS(-DQT_PLUGIN)
# ADD_DEFINITIONS(-DQT_NO_DEBUG)
# ADD_DEFINITIONS(-DQT_SHARED)
# 
# include_directories(
#   ${CMAKE_CURRENT_BINARY_DIR}
#   ../../
# )
# 
# set(rocs_KMLParser_SRCS KMLHandler.cpp KMLParser.cpp )
# 
# qt4_automoc(${rocs_KMLParser_SRCS})
# 
# kde4_add_plugin(rocs_kmlParser ${rocs_KMLParser_SRCS})
# 
# target_link_libraries(rocs_kmlParser
#   ${KDE4_KDEUI_LIBS}
#   ${QT_QTXML_LIBRARY}
#   rocslib
# )
# 
# install(TARGETS rocs_kmlParser  DESTINATION ${PLUGIN_INSTALL_DIR})
# 
# install( FILES rocs_kmlParser.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
# 
# if (KDE4_BUILD_TESTS)
#   enable_testing()
#   add_subdirectory(tests)
# endif (KDE4_BUILD_TESTS)
