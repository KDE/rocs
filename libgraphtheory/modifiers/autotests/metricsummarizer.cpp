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
