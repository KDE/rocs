project(librocsengine)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../rocs_core )
 
set( rocsengine_LIB_SRCS
  kross_rocsengine.cpp
)

set( rocsengine_LIB_HDRS
  kross_rocsengine.h rocsengine_export.h
)
 
kde4_add_library( librocsengine  STATIC ${rocsengine_LIB_SRCS} )