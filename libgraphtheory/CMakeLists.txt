# Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

include_directories(
    ./
)

add_definitions(-DTRANSLATION_DOMAIN=\"libgraphtheory\")


set(graphtheory_SRCS
    edge.cpp
    edgetype.cpp
    edgetypestyle.cpp
    graphdocument.cpp
    node.cpp
    nodetype.cpp
    editor.cpp
    view.cpp
    dialogs/nodeproperties.cpp
    dialogs/edgeproperties.cpp
    dialogs/nodetypeproperties.cpp
    dialogs/edgetypeproperties.cpp
    dialogs/propertieswidget.cpp
    dialogs/propertydelegate.cpp
    kernel/documentwrapper.cpp
    kernel/nodewrapper.cpp
    kernel/edgewrapper.cpp
    kernel/kernel.cpp
    kernel/modules/console/consolemodule.cpp
    models/nodemodel.cpp
    models/edgemodel.cpp
    models/nodepropertymodel.cpp
    models/edgepropertymodel.cpp
    models/nodetypemodel.cpp
    models/nodetypepropertymodel.cpp
    models/edgetypemodel.cpp
    models/edgetypepropertymodel.cpp
    modifiers/valueassign.cpp
    modifiers/topology.cpp
    fileformats/fileformatinterface.cpp
    fileformats/fileformatmanager.cpp
    editorplugins/editorplugininterface.cpp
    editorplugins/editorpluginmanager.cpp
    qtquickitems/nodeitem.cpp
    qtquickitems/edgeitem.cpp
    qtquickitems/qsgarrowheadnode.cpp
    qtquickitems/qsglinenode.cpp
)

ki18n_wrap_ui(graphtheory_SRCS
    dialogs/nodeproperties.ui
    dialogs/edgeproperties.ui
)

add_library(rocsgraphtheory SHARED ${graphtheory_SRCS} )

target_link_libraries(rocsgraphtheory
    PUBLIC
        Qt5::Core
        Qt5::Quick
        Qt5::QuickWidgets
        Qt5::Gui
        Qt5::Script
        KF5::I18n
        KF5::ItemViews
        KF5::Declarative
        KF5::XmlGui
        KF5::Service
)

set(rocscore_LIB_HDRS
    edge.h
    graphdocument.h
    node.h
)

set_target_properties(
    rocsgraphtheory
    PROPERTIES
        VERSION ${GENERIC_LIB_VERSION}
        SOVERSION ${GENERIC_LIB_SOVERSION}
)

install(TARGETS rocsgraphtheory  ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${rocscore_LIB_HDRS}  DESTINATION ${INCLUDE_INSTALL_DIR}/rocs  COMPONENT Devel)

# Boost requires exceptions
kde_source_files_enable_exceptions(modifiers/topology.cpp)

ecm_optional_add_subdirectory(fileformats)
ecm_optional_add_subdirectory(editorplugins)
ecm_optional_add_subdirectory(kernel)
ecm_optional_add_subdirectory(modifiers)
ecm_optional_add_subdirectory(qml)
ecm_optional_add_subdirectory(autotests)
ecm_optional_add_subdirectory(tests)
