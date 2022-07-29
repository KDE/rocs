# SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

include(ECMInstallIcons)

ecm_install_icons( ICONS
  128-apps-rocs.png  16-apps-rocs.png  22-apps-rocs.png  32-apps-rocs.png  48-apps-rocs.png  64-apps-rocs.png
  sc-apps-rocs.svgz

  sc-actions-rocsdelete.svgz
  sc-actions-rocsedge.svgz
  sc-actions-rocsnode.svgz
  sc-actions-rocsselect.svgz

  sc-actions-rocsbidirectional.svgz
  sc-actions-rocsunidirectional.svgz

  sc-actions-rocsadvancedsetup.svgz
  sc-actions-rocsvisible.svgz

  sc-actions-rocsaligntree.svgz
  sc-actions-rocsaligncircle.svgz
  sc-actions-rocsalignbottom.svgz
  sc-actions-rocsalignmiddle.svgz
  sc-actions-rocsalignleft.svgz
  sc-actions-rocsalignright.svgz
  sc-actions-rocsaligntop.svgz
  sc-actions-rocsalignvmiddle.svgz

  DESTINATION ${KDE_INSTALL_ICONDIR}
  THEME hicolor
)

target_sources(rocsgraphtheory PRIVATE
	icons.qrc
)
