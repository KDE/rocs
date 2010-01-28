
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../Core )
 
set( rocsmodel_LIB_SRCS 
  model_GraphDocument.cpp 
  model_GraphLayers.cpp
  model_GraphProperties.cpp
)

kde4_add_library( librocsmodel  STATIC ${rocsmodel_LIB_SRCS} )
