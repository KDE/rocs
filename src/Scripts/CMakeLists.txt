
FILE(GLOB ROCS_SCRIPT_FILES "*.js")


install(FILES
       ${ROCS_SCRIPT_FILES}
       DESTINATION  ${DATA_INSTALL_DIR}/rocs/examples
)