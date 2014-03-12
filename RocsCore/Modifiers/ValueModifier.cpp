/*
    This file is part of Rocs.
    Copyright (C) 2011 Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "ValueModifier.h"

#include <limits.h>

#include "Pointer.h"
#include "Data.h"

#include <QString>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

ValueModifier::ValueModifier()
{
}


template<typename T>
void ValueModifier::enumerate(const QList<T> &list, const QString &property, int start, const QString &baseString, bool overrideValues)
{
    for (int i = 0; i < list.size(); i++) {
        if (!overrideValues && !list[i]->property(property.toLatin1()).isNull()) {
            return;
        }
        list[i]->setProperty(property.toLatin1(), baseString + QString::number(start++));
    }
}
template ROCSLIB_EXPORT void ValueModifier::enumerate<DataPtr>(const QList<DataPtr> &list, const QString &property, int start, const QString &baseString, bool overrideValues);
template ROCSLIB_EXPORT void ValueModifier::enumerate<PointerPtr>(const QList<PointerPtr> &list, const QString &property, int start, const QString &baseString, bool overrideValues);


template<typename T>
void ValueModifier::enumerateAlpha(const QList< T >& list, const QString &property, const QString &start, bool overrideValues)
{
    QString identifier = start;
    for (int i = start.length()-1; i >= 0; --i) {
        // ensure that we only have letters
        if (!identifier.at(i).isLetter()) {
            identifier.replace(i, 1, 'a');
        }
    }

    for (int i = 0; i < list.size(); i++) {
        if (!overrideValues && !list[i]->property(property.toLatin1()).isNull()) {
            return;
        }
        list[i]->setProperty(property.toLatin1(), identifier);
        qDebug() << "XXX " << identifier;

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
template ROCSLIB_EXPORT void ValueModifier::enumerateAlpha<DataPtr>(const QList<DataPtr> &list, const QString &property, const QString &start, bool overrideValues);
template ROCSLIB_EXPORT void ValueModifier::enumerateAlpha<PointerPtr>(const QList<PointerPtr> &list, const QString &property, const QString &start, bool overrideValues);


template<typename T>
void ValueModifier::assignRandomIntegers(const QList<T> &list, const QString &property, int lowerLimit, int upperLimit, int seed, bool overrideValues)
{
    if (lowerLimit > upperLimit) {
        return;
    }

    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed));

    boost::uniform_int<> distribution(lowerLimit, upperLimit);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, distribution);

    for (int i = 0; i < list.size(); i++) {
        if (!overrideValues && !list[i]->property(property.toLatin1()).isNull()) {
            return;
        }
        list[i]->setProperty(property.toLatin1(), QString::number(die()));
    }
}
template ROCSLIB_EXPORT void ValueModifier::assignRandomIntegers<DataPtr>(const QList<DataPtr> &list, const QString &property, int lowerLimit, int upperLimit, int seed, bool overrideValues);
template ROCSLIB_EXPORT void ValueModifier::assignRandomIntegers<PointerPtr>(const QList<PointerPtr> &list, const QString &property, int lowerLimit, int upperLimit, int seed, bool overrideValues);


template<typename T>
void ValueModifier::assignRandomReals(const QList<T> &list, const QString &property, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues)
{
    if (lowerLimit > upperLimit) {
        return;
    }

    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed));

    boost::uniform_real<> distribution(lowerLimit, upperLimit);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > die(gen, distribution);

    for (int i = 0; i < list.size(); i++) {
        if (!overrideValues && !list[i]->property(property.toLatin1()).isNull()) {
            return;
        }
        list[i]->setProperty(property.toLatin1(), QString::number(die()));
    }
}
template ROCSLIB_EXPORT void ValueModifier::assignRandomReals<DataPtr>(const QList<DataPtr> &list, const QString &property, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues);
template ROCSLIB_EXPORT void ValueModifier::assignRandomReals<PointerPtr>(const QList<PointerPtr> &list, const QString &property, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues);

template<typename T>
void ValueModifier::assignConstantValue(const QList<T> &list, const QString &property, const QString &constant, bool overrideValues)
{
    for (int i = 0; i < list.size(); i++) {
        if (!overrideValues && !list[i]->property(property.toLatin1()).isNull()) {
            return;
        }
        list[i]->setProperty(property.toLatin1(), constant);
    }
}
template ROCSLIB_EXPORT void ValueModifier::assignConstantValue<DataPtr>(const QList<DataPtr> &list, const QString &property, const QString &constant, bool overrideValues);
template ROCSLIB_EXPORT void ValueModifier::assignConstantValue<PointerPtr>(const QList<PointerPtr> &list, const QString &property, const QString &constant, bool overrideValues);
