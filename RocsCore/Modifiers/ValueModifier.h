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
     * \param list QList of PointerPtr or DataPtr
     * \param property the property the shall be set to specified value
     * \param start the integer to start from
     * \param baseString the string to append the successive integers to
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    template<typename T>
    void enumerate(const QList<T>& list, const QString &property, int start, const QString &baseString, bool overrideValues);

    /**
     * Assign strings in increasing order starting at 'start' to all nodes.
     *
     * \param list QList of PointerPtr or DataPtr
     * \param property the property the shall be set to specified value
     * \param start the string to start from
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    template<typename T>
    void enumerateAlpha(const QList<T> &list, const QString &property, const QString &start, bool overrideValues);

    /**
     * Assign integers uniformly at random from range [lowerLimit,upperLimit] to data elements. Using Mersenne-Twister
     * random number generator, initialized by 'seed'. If not lowerLimit < upperLimit the function returns without
     * any operation.
     *
     * \param list QList of PointerPtr or DataPtr
     * \param property the property the shall be set to specified value
     * \param lowerLimit the lower limit for random number interval
     * \param upperLimit the upper limit for random number interval
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    template<typename T>
    void assignRandomIntegers(const QList<T> &list, const QString &property, int lowerLimit, int upperLimit, int seed, bool overrideValues);

    /**
     * Assign float values uniformly at random from range [lowerLimit,upperLimit] to nodes. Using Mersenne-Twister
     * random number generator, initialized by 'seed'. If not lowerLimit < upperLimit the function returns without
     * any operation.
     *
     * \param list QList of PointerPtr or DataPtr
     * \param property the property the shall be set to specified value
     * \param lowerLimit the lower limit for random number interval
     * \param upperLimit the upper limit for random number interval
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    template<typename T>
    void assignRandomReals(const QList<T> &list, const QString &property, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues);

    /**
     * Assign a constant string value to nodes/pointers
     *
     * \param list QList of PointerPtr or DataPtr
     * \param property the property the shall be set to specified value
     * \param constant a string to assign
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    template<typename T>
    void assignConstantValue(const QList<T> &list, const QString &property, const QString &constant, bool overrideValues);
};

#endif // VALUEMODIFIER_H
