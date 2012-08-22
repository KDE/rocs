/*
    This file is part of Rocs.
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>
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


#ifndef KMLFILEFORMATTEST_H
#define KMLFILEFORMATTEST_H

#include <QtCore/QObject>

class KmlFileFormatTest : public QObject
{
    Q_OBJECT
private slots:
    void init();

    void someTest();
};

#endif // KMLFILEFORMATTEST_H
