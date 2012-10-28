/*
    This file is part of Rocs.
    Copyright (C) 2011-2012  Andreas Cord-Landwehr <phoenixx@uni-paderborn.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef VALUEMODIFIER_H
#define VALUEMODIFIER_H

#include <QtCore/QList>

#include "RocsCoreExport.h"
#include "CoreTypes.h"


class ROCSLIB_EXPORT ValueModifier
{

public:
    ValueModifier();

    /**
     * Assign integers in increasing order starting at 'start' to all nodes.
     *
     * \param dataList the list of data that shall be enumerated
     * \param property the property the shall be set to specified value
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    void enumerate(QList<DataPtr> dataList, const QString& property, int start, bool overrideValues);

    /**
     * Assign integers in increasing order starting at 'start' to all edges.
     *
     * \param pointers the list of pointers that shall be enumerated
     * \param property the property the shall be set to specified value
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    void enumerate(QList<PointerPtr> pointers, const QString& property, int start, bool overrideValues);

    /**
     * Assign integers uniformly at random from range [lowerLimit,upperLimit] to data elements. Using Mersenne-Twister
     * random number generator, initialized by 'seed'. If not lowerLimit < upperLimit the function returns without
     * any operation.
     *
     * \param dataList the list of data that shall get random integer values
     * \param property the property the shall be set to specified value
     * \param lowerLimit the lower limit for random number interval
     * \param upperLimit the upper limit for random number interval
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    void assignRandomIntegers(QList<DataPtr> dataList, const QString& property, int lowerLimit, int upperLimit, int seed, bool overrideValues);

    /**
     * Assign integers uniformly at random from range [lowerLimit,upperLimit] to edges. Using Mersenne-Twister
     * random number generator, initialized by 'seed'. If not lowerLimit < upperLimit the function returns without
     * any operation.
     *
     * \param pointers the list of pointers that shall get random integer values
     * \param property the property the shall be set to specified value
     * \param lowerLimit the lower limit for random number interval
     * \param upperLimit the upper limit for random number interval
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    void assignRandomIntegers(QList<PointerPtr> pointers, const QString& property, int lowerLimit, int upperLimit, int seed, bool overrideValues);

    /**
     * Assign float values uniformly at random from range [lowerLimit,upperLimit] to nodes. Using Mersenne-Twister
     * random number generator, initialized by 'seed'. If not lowerLimit < upperLimit the function returns without
     * any operation.
     *
     * \param dataList the list of data that shall get random float values
     * \param property the property the shall be set to specified value
     * \param lowerLimit the lower limit for random number interval
     * \param upperLimit the upper limit for random number interval
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    void assignRandomReals(QList<DataPtr> dataList, const QString& property, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues);

    /**
     * Assign float values uniformly at random from range [lowerLimit,upperLimit] to edges. Using Mersenne-Twister
     * random number generator, initialized by 'seed'. If not lowerLimit < upperLimit the function returns without
     * any operation.
     *
     * \param pointers the list of pointers that shall get random float values
     * \param property the property the shall be set to specified value
     * \param lowerLimit the lower limit for random number interval
     * \param upperLimit the upper limit for random number interval
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    void assignRandomReals(QList<PointerPtr> pointers, const QString& property, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues);
};

#endif // VALUEMODIFIER_H
