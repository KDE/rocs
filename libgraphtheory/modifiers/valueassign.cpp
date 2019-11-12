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

#include "valueassign.h"
#include "node.h"
#include "edge.h"
#include <limits.h>
#include <QString>
#include <QVariant>
#include <QVector>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

using namespace GraphTheory;

ValueAssign::ValueAssign()
{
}


template<typename T>
void ValueAssign::enumerate(const QVector<T> &list, const QString &property, int start, const QString &baseString, bool overrideValues)
{
    for (int i = 0; i < list.size(); i++) {
        if (!overrideValues && !list[i]->dynamicProperty(property).isNull()) {
            return;
        }
        list[i]->setDynamicProperty(property, baseString + QString::number(start++));
    }
}
template GRAPHTHEORY_EXPORT void ValueAssign::enumerate<NodePtr>(const QVector<NodePtr> &list, const QString &property, int start, const QString &baseString, bool overrideValues);
template GRAPHTHEORY_EXPORT void ValueAssign::enumerate<EdgePtr>(const QVector<EdgePtr> &list, const QString &property, int start, const QString &baseString, bool overrideValues);


template<typename T>
void ValueAssign::enumerateAlpha(const QVector< T >& list, const QString &property, const QString &start, bool overrideValues)
{
    QString identifier = start;
    for (int i = start.length()-1; i >= 0; --i) {
        // ensure that we only have letters
        if (!identifier.at(i).isLetter()) {
            identifier.replace(i, 1, 'a');
        }
    }

    for (int i = 0; i < list.size(); i++) {
        if (!overrideValues && !list[i]->dynamicProperty(property).isNull()) {
            return;
        }
        list[i]->setDynamicProperty(property, identifier);

        // compute new identifier by lexicographical increasing
        for (int i = identifier.length()-1; i >= 0; --i) {
            // ensure that we only have letters
            if (identifier.at(i) != 'Z') {
                identifier.replace(i, 1, QChar(identifier.at(i).toLatin1() + 1));
                break; // we are done, do not loop further
            } else {
                identifier.replace(i, 1, 'a');
            }
            if (i == 0 && identifier.at(0) == 'a') {
                identifier.append('a');
            }
        }
    }
}
template GRAPHTHEORY_EXPORT void ValueAssign::enumerateAlpha<NodePtr>(const QVector<NodePtr> &list, const QString &property, const QString &start, bool overrideValues);
template GRAPHTHEORY_EXPORT void ValueAssign::enumerateAlpha<EdgePtr>(const QVector<EdgePtr> &list, const QString &property, const QString &start, bool overrideValues);


template<typename T>
void ValueAssign::assignRandomIntegers(const QVector<T> &list, const QString &property, int lowerLimit, int upperLimit, int seed, bool overrideValues)
{
    if (lowerLimit > upperLimit) {
        return;
    }

    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed));

    boost::uniform_int<> distribution(lowerLimit, upperLimit);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, distribution);

    for (int i = 0; i < list.size(); i++) {
        if (!overrideValues && !list[i]->dynamicProperty(property).isNull()) {
            return;
        }
        list[i]->setDynamicProperty(property, QString::number(die()));
    }
}
template GRAPHTHEORY_EXPORT void ValueAssign::assignRandomIntegers<NodePtr>(const QVector<NodePtr> &list, const QString &property, int lowerLimit, int upperLimit, int seed, bool overrideValues);
template GRAPHTHEORY_EXPORT void ValueAssign::assignRandomIntegers<EdgePtr>(const QVector<EdgePtr> &list, const QString &property, int lowerLimit, int upperLimit, int seed, bool overrideValues);


template<typename T>
void ValueAssign::assignRandomReals(const QVector<T> &list, const QString &property, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues)
{
    if (lowerLimit > upperLimit) {
        return;
    }

    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed));

    boost::uniform_real<> distribution(lowerLimit, upperLimit);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > die(gen, distribution);

    for (int i = 0; i < list.size(); i++) {
        if (!overrideValues && !list[i]->dynamicProperty(property).isNull()) {
            return;
        }
        list[i]->setDynamicProperty(property, QString::number(die()));
    }
}
template GRAPHTHEORY_EXPORT void ValueAssign::assignRandomReals<NodePtr>(const QVector<NodePtr> &list, const QString &property, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues);
template GRAPHTHEORY_EXPORT void ValueAssign::assignRandomReals<EdgePtr>(const QVector<EdgePtr> &list, const QString &property, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues);

template<typename T>
void ValueAssign::assignConstantValue(const QVector<T> &list, const QString &property, const QString &constant, bool overrideValues)
{
    for (int i = 0; i < list.size(); i++) {
        if (!overrideValues && !list[i]->dynamicProperty(property).isNull()) {
            return;
        }
        list[i]->setDynamicProperty(property, constant);
    }
}
template GRAPHTHEORY_EXPORT void ValueAssign::assignConstantValue<NodePtr>(const QVector<NodePtr> &list, const QString &property, const QString &constant, bool overrideValues);
template GRAPHTHEORY_EXPORT void ValueAssign::assignConstantValue<EdgePtr>(const QVector<EdgePtr> &list, const QString &property, const QString &constant, bool overrideValues);
