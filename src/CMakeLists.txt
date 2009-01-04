project(Rocs-rewrite)

find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} Interface)


add_subdirectory(Core)
add_subdirectory(Models)
add_subdirectory(GraphicsItem)
add_subdirectory(Interface)
add_subdirectory(Tests)

set(Rocs_SRCS  Main.cpp )

# Resource Files
# set (Rocs_RCCS
#    resources.qrc
# )
# qt4_add_resources(Rocs_RCC_SRCS ${Rocs_RCCS})

# Since the JavaScript supports came from QtScript... 

set (QT_USE_QTSCRIPT true)

set(Rocs_KCFGS settings.kcfgc)

kde4_add_kcfg_files( Rocs_SRCS ${Rocs_KCFGS} )
kde4_add_executable(Rocs ${Rocs_SRCS} ${Rocs_RCC_SRCS})

target_link_libraries(Rocs ${KDE4_KDEUI_LIBS}
			${KDE4_KROSSUI_LIBS}
      ${KDE4_KIO_LIBS}
      ${KDE4_KTEXTEDITOR_LIBS}
      ${QT_QTSCRIPT_LIBRARY}
			
			librocsgraphics
			librocsui
			librocsmodel
			librocscore
)

install( TARGETS Rocs  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES Rocs.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES Rocsui.rc DESTINATION ${DATA_INSTALL_DIR}/Rocs  )
install( FILES Rocs.kcfg DESTINATION  ${KCFG_INSTALL_DIR} )

