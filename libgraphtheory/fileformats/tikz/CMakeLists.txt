##
#   This file is part of Rocs.
#   Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>
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
# ##

set(tikzformat_SRCS
    tikzfileformat.cpp
)

add_library(tikzfileformat MODULE ${tikzformat_SRCS})

target_link_libraries(tikzfileformat
    rocsgraphtheory
)

kservice_desktop_to_json(tikzfileformat tikzfileformat.desktop)
install(TARGETS tikzfileformat DESTINATION ${PLUGIN_INSTALL_DIR}/rocs/fileformats)

# ecm_optional_add_subdirectory(autotests)
