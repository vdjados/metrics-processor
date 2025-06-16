#ifndef METRIC_H
#define METRIC_H

#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

class Metric {
public:
    Metric(const std::string& name) : name_(name) {}
    virtual ~Metric() = default;

    const std::string& getName() const { return name_; }

    virtual void record(double value) = 0;
    virtual void record(long long value) = 0;
    virtual std::string getValueAsStringAndReset() = 0;

protected:
    std::string name_;
};

#endif // METRIC_H 