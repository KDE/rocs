project(Rocs)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
find_package(KDE4 REQUIRED)
find_package(KDEEdu)
include(KDE4Defaults)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES})

add_subdirectory(src-rewrite)