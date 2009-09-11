project(Rocs)

cmake_minimum_required(VERSION 2.6.0)
set(KDE_MIN_VERSION "4.3")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      ${CMAKE_SOURCE_DIR}/cmake/modules
)

find_package(KDE4 REQUIRED)
#find_package(GraphViz REQUIRED)

include(KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_TOLOWER)
if(CMAKE_BUILD_TYPE_TOLOWER MATCHES debugfull)
  add_definitions (-DQT_STRICT_ITERATORS)
endif()

include_directories(
  ${CMAKE_SOURCE_DIR} 
  ${CMAKE_BINARY_DIR} 
  ${KDE4_INCLUDES} 
  ${KDE4_INCLUDE_DIR} 
  ${QT_INCLUDES}
)

add_subdirectory(src)
add_subdirectory(graphics)

if(CMAKE_SOURCE_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    macro_display_feature_log()
endif(CMAKE_SOURCE_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
