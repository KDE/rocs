/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
