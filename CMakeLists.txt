project(Rocs)

cmake_minimum_required(VERSION 2.6.0)
set(KDE_MIN_VERSION "4.3")

find_package(KDE4 REQUIRED)

include(KDE4Defaults)
remove_definitions(-DQT_NO_STL)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

include_directories(
  ${KDE4_INCLUDES}
  ${QT_INCLUDES}
)

if(${CMAKE_BUILD_TYPE} MATCHES coverage)
  set (CMAKE_CXX_COMPILER cov++)
endif()

#### Boost ####
# In CMake >= 2.8.6, FindBoost.cmake tries to find BoostConfig.cmake which is
# not compatible with CMake's FindBoost. Disable this function.
set (Boost_NO_BOOST_CMAKE TRUE)

find_package(Boost "1.39" REQUIRED)
macro_log_feature(Boost_FOUND "Boost C++ Libraries" "Required to build Rocs" "http://www.boost.org" TRUE "1.39" "")

if (Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})

   add_subdirectory(doc)
   add_subdirectory(src)
   add_subdirectory(graphics)
endif (Boost_FOUND)

if(CMAKE_SOURCE_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    macro_display_feature_log()
endif(CMAKE_SOURCE_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
