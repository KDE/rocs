project(rocs)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} Interface)

add_subdirectory(Core)
add_subdirectory(Models)
add_subdirectory(GraphicsItem)
add_subdirectory(Interface)
add_subdirectory(Tests)

set(rocs_SRCS  Main.cpp )

# Since the JavaScript supports came from QtScript... 

set (QT_USE_QTSCRIPT true)

set(rocs_KCFGS settings.kcfgc)

kde4_add_kcfg_files(rocs_SRCS ${rocs_KCFGS} )
kde4_add_executable(rocs ${rocs_SRCS} ${rocs_RCC_SRCS})

target_link_libraries(rocs ${KDE4_KDEUI_LIBS}
	${KDE4_KROSSUI_LIBS}
      ${KDE4_KIO_LIBS}
      ${KDE4_KTEXTEDITOR_LIBS}
      ${QT_QTSCRIPT_LIBRARY}

	librocsui
	librocsgraphics
	librocsmodel
	librocscore
)

install( TARGETS rocs  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES rocs.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES rocsui.rc DESTINATION ${DATA_INSTALL_DIR}/rocs  )
install( FILES rocs.kcfg DESTINATION  ${KCFG_INSTALL_DIR} )

