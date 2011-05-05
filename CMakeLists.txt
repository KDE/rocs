project(Rocs)

cmake_minimum_required(VERSION 2.6.0)
set(KDE_MIN_VERSION "4.3")

find_package(KDE4 REQUIRED)

include(KDE4Defaults)
remove_definitions(-DQT_NO_STL)

include_directories(
  ${KDE4_INCLUDES}
  ${QT_INCLUDES}
)

if(${CMAKE_BUILD_TYPE} MATCHES coverage)
  set (CMAKE_CXX_COMPILER cov++)
  set (CMAKE_CXX_FLAGS "${KDE4_ENABLE_EXCEPTIONS}" )
endif()


add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(graphics)

if(CMAKE_SOURCE_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    macro_display_feature_log()
endif(CMAKE_SOURCE_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
