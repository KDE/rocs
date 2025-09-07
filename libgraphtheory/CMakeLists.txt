# SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
# SPDX-License-Identifier: BSD-2-Clause

# we use SOVERION 0 to make clear that this is neither stable API nor stable ABI
# i.e., the library for now is only used internally in Rocs and we require the exact
# version as released alongside Rocs
set(GRAPHTHEORY_LIB_VERSION "0.0.1")
set(GRAPHTHEORY_LIB_SOVERSION "0")

add_library(rocsgraphtheory SHARED)
set_target_properties(rocsgraphtheory PROPERTIES
#         VERSION ${GRAPHTHEORY_LIB_VERSION}
        SOVERSION ${GRAPHTHEORY_LIB_SOVERSION}
)

target_sources(rocsgraphtheory PRIVATE
    edge.cpp
    edgetype.cpp
    edgetypestyle.cpp
    graphdocument.cpp
    logging.cpp
    node.cpp
    nodetype.cpp
    nodetypestyle.cpp
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
    kernel/engine.cpp
    kernel/engine.h
    kernel/kerneltypes.h
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
    fileformats/fileformatmanager.h
    fileformats/fileformatmanager_p.h
    editorplugins/editorplugininterface.cpp
    editorplugins/editorpluginmanager.cpp
    qtquickitems/nodeitem.cpp
    qtquickitems/edgeitem.cpp
    qtquickitems/qsgarrowheadnode.cpp
    qtquickitems/qsglinenode.cpp
    qml/rocs.qrc
)

ki18n_wrap_ui(rocsgraphtheory
    dialogs/nodeproperties.ui
    dialogs/edgeproperties.ui
)

generate_export_header(rocsgraphtheory BASE_NAME graphtheory)

add_subdirectory(fileformats)

target_link_libraries(rocsgraphtheory
    PUBLIC
        Qt::Core
        Qt::Quick
        Qt::QuickWidgets
        Qt::Gui
        KF6::CoreAddons
        KF6::I18n
        KF6::ItemViews
        KF6::XmlGui
)

set(rocscore_LIB_HDRS
    edge.h
    graphdocument.h
    node.h
)

# KI18N Translation Domain for library
add_definitions(-DTRANSLATION_DOMAIN=\"libgraphtheory\")

install(TARGETS rocsgraphtheory  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ${rocscore_LIB_HDRS}  DESTINATION ${KDE_INSTALL_INCLUDEDIR}/rocs  COMPONENT Devel)

# Boost requires exceptions
kde_source_files_enable_exceptions(modifiers/topology.cpp modifiers/valueassign.cpp)

ecm_optional_add_subdirectory(editorplugins)
ecm_optional_add_subdirectory(kernel)
ecm_optional_add_subdirectory(modifiers)
ecm_optional_add_subdirectory(qml)

if(BUILD_TESTING)
    ecm_optional_add_subdirectory(autotests)
    ecm_optional_add_subdirectory(tests)
    ecm_optional_add_subdirectory(modifiers/autotests)
endif()
