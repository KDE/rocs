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
##

add_subdirectory(tgfFileFormat)
add_subdirectory(tikzFileFormat)

# require Boost >= 1.49 for DOT file format
if(${Boost_VERSION} VERSION_LESS 104900)
    add_feature_info(DotFileFormat
        OFF
        "Building of DOT file format support needs Boost >= 1.49."
    )
else(${Boost_VERSION} VERSION_LESS 104900)
    add_subdirectory(dotFileFormat)
endif(${Boost_VERSION} VERSION_LESS 104900)

# these plugins are in an unstable state and are not suited for end users
if (ROCS_BUILD_LOADSAVE)
    add_subdirectory(kmlFileFormat)
    add_subdirectory(gmlFileFormat)
endif(ROCS_BUILD_LOADSAVE)


