##
#   This file is part of Rocs.
#   Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
#   Copyright 2010-2012  Wagner Reck <wagner.reck@gmail.com>
#   Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>
#
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License as
#   published by the Free Software Foundation; either version 2 of
#   the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

# Prepare library.
set( rocs_core_SRCS
    DataType.cpp
    PointerType.cpp
    Data.cpp
    DataStructure.cpp
    Document.cpp
    Group.cpp
    Project.cpp
    DataStructurePluginInterface.cpp
    DataStructureBackendManager.cpp
    DocumentManager.cpp
    KrossBackend.cpp
    Pointer.cpp
    QtScriptBackend.cpp
    Modifiers/Topology.cpp
    Modifiers/ValueModifier.cpp

    IncludeManager.cpp

    LoadSave/GraphFileBackendManager.cpp
    LoadSave/GraphFilePluginInterface.cpp
    LoadSave/RocsGraphFileFormatPlugin.cpp
)

kde4_add_library( rocscore  SHARED ${rocs_core_SRCS} )

target_link_libraries( rocscore
    ${KDE4_KROSSCORE_LIBS}
    ${KDE4_KIO_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTSCRIPTTOOLS_LIBRARY}
)

set( rocscore_LIB_HDRS
    Data.h
    DataStructure.h
    Document.h
    Pointer.h
    CoreTypes.h
    RocsCoreExport.h
    DataStructurePluginInterface.h
    DataStructureBackendManager.h

    LoadSave/GraphFileBackendManager.h
    LoadSave/GraphFilePluginInterface.h

    Modifiers/Topology.h
)

set_target_properties( rocscore  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS rocscore  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES ${rocscore_LIB_HDRS}  DESTINATION ${INCLUDE_INSTALL_DIR}/rocs  COMPONENT Devel )

################## INSTALLS ##########################

install( FILES
    RocsDataStructurePlugin.desktop
    LoadSave/RocsGraphFilePlugin.desktop
    DESTINATION
    ${SERVICETYPES_INSTALL_DIR}
)

add_subdirectory(DataStructures)
add_subdirectory(LoadSave)

install( FILES
    RocsDataStructurePlugin.desktop
    LoadSave/RocsGraphFilePlugin.desktop
    DESTINATION
    ${SERVICETYPES_INSTALL_DIR}
)

# Build Tests if KDE_TEST is on
if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)
