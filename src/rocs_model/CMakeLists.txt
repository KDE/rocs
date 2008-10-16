project(librocsmodel)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../rocs_core )
 
set( rocsmodel_LIB_SRCS  model_GraphDocument.cpp model_GraphLayers.cpp
)

kde4_add_library( librocsmodel  STATIC ${rocsmodel_LIB_SRCS} )
