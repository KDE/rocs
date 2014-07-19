/*
    This file is part of Rocs.
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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


#ifndef VISUALEDITOR_EXPORT_H
#define VISUALEDITOR_EXPORT_H

#define KDE_EXPORT __attribute__ ((visibility("default")))
#define KDE_IMPORT __attribute__ ((visibility("default")))

#ifndef VISUALEDITOR_EXPORT
# if defined(MAKE_ROCSVISUALEDITOR_LIB)
// We are building this library
#  define VISUALEDITOR_EXPORT KDE_EXPORT
# else
// We are using this library
#  define VISUALEDITOR_EXPORT KDE_IMPORT
# endif
#endif

# ifndef VISUALEDITOR_EXPORT_DEPRECATED
#  define VISUALEDITOR_EXPORT_DEPRECATED KDE_DEPRECATED VISUALEDITOR_EXPORT
# endif

#endif
