#add_subdirectory(MakeComplete)

add_subdirectory(importPlainTXT)

add_subdirectory(kmlParser)

add_subdirectory(DataStructure)

# PLUGIN: graph generator
# requires boost version >= 1.42
find_package(Boost)
IF("${Boost_VERSION}" VERSION_LESS 104020)
   message(STATUS, "compiled without graph-generator plugin, BOOST reqested at least 1.39")
ELSE("${Boost_VERSION}" VERSION_LESS 104020)
   add_subdirectory(GenerateGraph)
   message(STATUS, "compiled with graph-generator plugin")
ENDIF("${Boost_VERSION}" VERSION_LESS 104020)

include_directories(${Boost_INCLUDE_DIRS})

#removed until fix build with Boost
#add_subdirectory(GMLParser)
#add_subdirectory(dotParser)

add_subdirectory(TransformEdges)