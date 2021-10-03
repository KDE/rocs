# SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

install(FILES kernelapi.xsd DESTINATION ${KDE_INSTALL_DATADIR}/rocs/schemes)
install(FILES modules/console/console.xml DESTINATION ${KDE_INSTALL_DATADIR}/rocs/kernelapi)
install(FILES modules/document/document.xml DESTINATION ${KDE_INSTALL_DATADIR}/rocs/kernelapi)
install(FILES modules/document/node.xml DESTINATION ${KDE_INSTALL_DATADIR}/rocs/kernelapi)
install(FILES modules/document/edge.xml DESTINATION ${KDE_INSTALL_DATADIR}/rocs/kernelapi)

if(BUILD_TESTING)
# also copy all files for autotests
file(COPY kernelapi.xsd DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../autotests/kernelapi)
file(COPY modules/console/console.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../autotests/kernelapi)
file(COPY modules/document/document.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../autotests/kernelapi)
file(COPY modules/document/node.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../autotests/kernelapi)
file(COPY modules/document/edge.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../autotests/kernelapi)
endif()
