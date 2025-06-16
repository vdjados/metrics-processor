#ifndef GAUGE_METRIC_H
#define GAUGE_METRIC_H

#include "Metric.h"
#include <atomic>
#include <mutex>
#include <string>

class GaugeMetric : public Metric {
public:
    GaugeMetric(const std::string& name) : Metric(name), value_(0.0) {}

    void record(double value) {
        std::lock_guard<std::mutex> lock(mtx_);
        value_ = value;
    }
    void record(long long value) { /* Not applicable for gauge */ }

    std::string getValueAsStringAndReset() {
        std::lock_guard<std::mutex> lock(mtx_);
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << value_;
        value_ = 0.0;
        return ss.str();
    }

private:
    double value_;
    std::mutex mtx_;
};

#endif // GAUGE_METRIC_H 