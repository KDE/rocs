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

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

ValueModifier::ValueModifier()
{
}


void ValueModifier::enumerate(QList<DataPtr> dataList, const QString& property, int start, bool overrideValues = true)
{
    for (int i = 0; i < dataList.size(); i++) {
        if (!overrideValues && !dataList[i]->property(property.toStdString().c_str()).isNull()) {
            return;
        }
        dataList[i]->setProperty(property.toStdString().c_str(), QString::number(start++));
    }
}


void ValueModifier::enumerate(QList<PointerPtr> pointers, const QString& property, int start, bool overrideValues = true)
{
    for (int i = 0; i < pointers.size(); i++) {
        if (!overrideValues && !pointers[i]->property(property.toStdString().c_str()).isNull()) {
            return;
        }
        pointers[i]->setProperty(property.toStdString().c_str(), QString::number(start++));
    }
}


void ValueModifier::assignRandomIntegers(QList<DataPtr> dataList, const QString& property, int lowerLimit, int upperLimit, int seed, bool overrideValues = true)
{
    if (lowerLimit > upperLimit)
        return;

    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed));

    boost::uniform_int<> distribution(lowerLimit, upperLimit);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, distribution);

    for (int i = 0; i < dataList.size(); i++) {
        if (!overrideValues && !dataList[i]->property(property.toStdString().c_str()).isNull()) {
            return;
        }
        dataList[i]->setProperty(property.toStdString().c_str(), QString::number(die()));
    }
}


void ValueModifier::assignRandomIntegers(QList<PointerPtr> pointers, const QString& property, int lowerLimit, int upperLimit, int seed, bool overrideValues = true)
{
    if (lowerLimit > upperLimit) {
        return;
    }

    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed));

    boost::uniform_int<> distribution(lowerLimit, upperLimit);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, distribution);

    for (int i = 0; i < pointers.size(); i++) {
        if (!overrideValues && !pointers[i]->property(property.toStdString().c_str()).isNull()) {
            return;
        }
        pointers[i]->setProperty(property.toStdString().c_str(), QString::number(die()));
    }
}


void ValueModifier::assignRandomReals(QList<DataPtr> dataList, const QString& property, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues = true)
{
    if (lowerLimit > upperLimit) {
        return;
    }

    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed));

    boost::uniform_real<> distribution(lowerLimit, upperLimit);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > die(gen, distribution);

    for (int i = 0; i < dataList.size(); i++) {
        if (!overrideValues && !dataList[i]->property(property.toStdString().c_str()).isNull()) {
            return;
        }
        dataList[i]->setProperty(property.toStdString().c_str(), QString::number(die()));
    }
}


void ValueModifier::assignRandomReals(QList<PointerPtr> pointers, const QString& property, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues = true)
{
    if (lowerLimit > upperLimit) {
        return;
    }

    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed));

    boost::uniform_real<> distribution(lowerLimit, upperLimit);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > die(gen, distribution);

    for (int i = 0; i < pointers.size(); i++) {
        if (!overrideValues && !pointers[i]->property(property.toStdString().c_str()).isNull()) {
            return;
        }
        pointers[i]->setProperty(property.toStdString().c_str(), QString::number(die()));
    }
}
