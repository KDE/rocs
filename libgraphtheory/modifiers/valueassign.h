/*
 *  Copyright 2011-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef VALUEASSIGN_H
#define VALUEASSIGN_H

#include "typenames.h"
#include "graphtheory_export.h"

namespace GraphTheory
{

/** \brief this class provides value assigner
 */
class GRAPHTHEORY_EXPORT ValueAssign
{

public:
    ValueAssign();
    /**
     * Assign integers in increasing order starting at 'start' to all nodes.
     *
     * \param list QVector of EdgePtr or NodePtr
     * \param property the property the shall be set to specified value
     * \param start the integer to start from
     * \param baseString the string to append the successive integers to
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    template<typename T>
    void enumerate(const QVector<T>& list, const QString &property, int start, const QString &baseString, bool overrideValues = true);

    /**
     * Assign strings in increasing order starting at 'start' to all nodes.
     *
     * \param list QVector of EdgePtr or NodePtr
     * \param property the property the shall be set to specified value
     * \param start the string to start from
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    template<typename T>
    void enumerateAlpha(const QVector<T> &list, const QString &property, const QString &start, bool overrideValues = true);

    /**
     * Assign integers uniformly at random from range [lowerLimit,upperLimit] to data elements. Using Mersenne-Twister
     * random number generator, initialized by 'seed'. If not lowerLimit < upperLimit the function returns without
     * any operation.
     *
     * \param list QVector of EdgePtr or NodePtr
     * \param property the property the shall be set to specified value
     * \param lowerLimit the lower limit for random number interval
     * \param upperLimit the upper limit for random number interval
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    template<typename T>
    void assignRandomIntegers(const QVector<T> &list, const QString &property, int lowerLimit, int upperLimit, int seed, bool overrideValues = true);

    /**
     * Assign float values uniformly at random from range [lowerLimit,upperLimit] to nodes. Using Mersenne-Twister
     * random number generator, initialized by 'seed'. If not lowerLimit < upperLimit the function returns without
     * any operation.
     *
     * \param list QVector of EdgePtr or NodePtr
     * \param property the property the shall be set to specified value
     * \param lowerLimit the lower limit for random number interval
     * \param upperLimit the upper limit for random number interval
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    template<typename T>
    void assignRandomReals(const QVector<T> &list, const QString &property, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues = true);

    /**
     * Assign a constant string value to nodes/pointers
     *
     * \param list QVector of EdgePtr or NodePtr
     * \param property the property the shall be set to specified value
     * \param constant a string to assign
     * \param overrideValues if true, current property values are overwritten; otherwise not
     */
    template<typename T>
    void assignConstantValue(const QVector<T> &list, const QString &property, const QString &constant, bool overrideValues = true);
};
}

#endif
