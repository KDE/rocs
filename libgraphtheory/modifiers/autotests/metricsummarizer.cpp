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

#include "metricsummarizer.h"

#include <algorithm>

void MetricSummarizer::addValue(const qreal value) {
    values_m.push_back(value);
}

qreal MetricSummarizer::minimum() const {
    assert(not values_m.empty());
    return *std::min_element(values_m.begin(), values_m.end());
}

qreal MetricSummarizer::maximum() const {
    assert(not values_m.empty());
    return *std::max_element(values_m.begin(), values_m.end());
}

qreal MetricSummarizer::average() const {
    assert(not values_m.empty());
    return std::accumulate(values_m.begin(), values_m.end(), 0.) / values_m.size();
}
