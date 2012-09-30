/*
    This file is part of Rocs.
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */  

#ifndef ROCS_VERSION_H
#define ROCS_VERSION_H

#include <kdeversion.h>

#define ROCS_MAJOR_VERSION @ROCS_VERSION_MAJOR@
#define ROCS_MINOR_VERSION @ROCS_VERSION_MINOR@
#define ROCS_PATCH_VERSION @ROCS_VERSION_PATCH@

#define ROCS_VERSION_STR "@ROCS_VERSION_MAJOR@.@ROCS_VERSION_MINOR@.@ROCS_VERSION_PATCH@"

#define ROCS_VERSION KDE_MAKE_VERSION(@ROCS_VERSION_MAJOR@, @ROCS_VERSION_MINOR@, @ROCS_VERSION_PATCH@)

#endif
