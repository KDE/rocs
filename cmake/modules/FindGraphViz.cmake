# This cmake file comes from MOAB:
# MOAB, a Mesh-Oriented datABase, is a software component for creating,
# storing and accessing finite element mesh data.
# 
# Copyright 2004 Sandia Corporation.  Under the terms of Contract
# DE-AC04-94AL85000 with Sandia Coroporation, the U.S. Government
# retains certain rights in this software.
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.  A copy of the full
# GNU Lesser General Public License can be found at
# http://www.gnu.org/copyleft/lesser.html.
# 
# For more information, contact the authors of this software at
# moab@sandia.gov.

if (NOT WIN32)
  find_package(PkgConfig)
  pkg_check_modules(graphviz ${REQUIRED} libgvc libcdt libgraph libpathplan)
  if (GraphViz_FOUND)
    set (GraphViz_INCLUDE_DIRECTORIES ${GraphViz_INCLUDE_DIRS})
  endif (GraphViz_FOUND)
endif (NOT WIN32)
    
find_path(GraphViz_INCLUDE_DIRECTORIES
  NAMES gvc.h
  PATHS
  ${GraphViz_INCLUDE_DIRS}
  /usr/local/include
  /usr/include
  /usr/local/include/graphviz
  /usr/include/graphviz
)
    
find_library(GraphViz_GVC_LIBRARY
  NAMES gvc
  PATHS
  ${GraphViz_LIBRARY_DIRS}
  /usr/local/lib64
  /usr/lib64
  /usr/local/lib
  /usr/lib
)

find_library(GraphViz_CDT_LIBRARY
  NAMES cdt
  PATHS
  ${GraphViz_LIBRARY_DIRS}
  /usr/local/lib64
  /usr/lib64
  /usr/local/lib
  /usr/lib
)

find_library(GraphViz_GRAPH_LIBRARY
  NAMES graph
  PATHS
  ${GraphViz_LIBRARY_DIRS}
  /usr/local/lib64
  /usr/lib64
  /usr/local/lib
  /usr/lib
)

find_library(GraphViz_PATHPLAN_LIBRARY
  NAMES pathplan
  PATHS
  ${GraphViz_LIBRARY_DIRS}
  /usr/local/lib64
  /usr/lib64
  /usr/local/lib
  /usr/lib
)

if (GraphViz_INCLUDE_DIRECTORIES AND
    GraphViz_GVC_LIBRARY AND GraphViz_CDT_LIBRARY AND
    GraphViz_GRAPH_LIBRARY AND GraphViz_PATHPLAN_LIBRARY)
  set (GraphViz_FOUND 1)
  set (GraphViz_LIBRARIES
       "${GraphViz_GVC_LIBRARY};${GraphViz_GRAPH_LIBRARY};"
       "${GraphViz_CDT_LIBRARY};${GraphViz_PATHPLAN_LIBRARY}"
       CACHE FILEPATH "Libraries for graphviz")
else (GraphViz_INCLUDE_DIRECTORIES AND
      GraphViz_GVC_LIBRARY AND GraphViz_CDT_LIBRARY AND
      GraphViz_GRAPH_LIBRARY AND GraphViz_PATHPLAN_LIBRARY)
  set (GraphViz_FOUND 0)
  if (GraphViz_FIND_REQUIRED)
    message (FATAL_ERROR "GraphViz not installed !")
  endif (GraphViz_FIND_REQUIRED)
endif (GraphViz_INCLUDE_DIRECTORIES AND
GraphViz_GVC_LIBRARY AND GraphViz_CDT_LIBRARY AND
GraphViz_GRAPH_LIBRARY AND GraphViz_PATHPLAN_LIBRARY)
