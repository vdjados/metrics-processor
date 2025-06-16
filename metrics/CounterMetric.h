#ifndef COUNTER_METRIC_H
#define COUNTER_METRIC_H

#include "Metric.h"
#include <atomic>

class CounterMetric : public Metric {
public:
    CounterMetric(const std::string& name) : Metric(name), value_(0) {}

    void record(double value) override { /* Not applicable for counter */ }
    void record(long long value) override {
        value_.fetch_add(value, std::memory_order_relaxed);
    }

    std::string getValueAsStringAndReset() override {
        long long current_value = value_.exchange(0, std::memory_order_relaxed);
        return std::to_string(current_value);
    }

private:
    std::atomic<long long> value_;
};

#endif // COUNTER_METRIC_H 