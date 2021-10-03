/*
 *  SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    Q_OBJECT
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
