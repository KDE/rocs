/*
 *  Copyright 2020  Dilson Almeida Guimarães <dilsonguim@gmail.com>
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

#ifndef METRICSUMMARIZER_H
#define METRICSUMMARIZER_H

#include <QObject>
#include <QVector>

/*
 *Keeps track of metric values and calculate statistics about them.
 */
class MetricSummarizer : public QObject
{
    Q_OBJECT;
private:
    QVector<qreal> values_m;
public:
    /*
     * Adds a new metric value.
     */
    void addValue(const qreal value);
   
    /*
     * Calculates the minimum value already added.
     */
    qreal minimum() const;
    
    /*
     * Calculates the maximum value already added.
     */
    qreal maximum() const;
    
    /*
     * Calculates the average of the added values.
     */
    qreal average() const;
};

 #endif
