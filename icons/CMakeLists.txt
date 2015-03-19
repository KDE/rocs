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

  DESTINATION ${ICON_INSTALL_DIR}
  THEME hicolor
)
