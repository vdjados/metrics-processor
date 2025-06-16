#ifndef METRICS_COLLECTOR_H
#define METRICS_COLLECTOR_H

#include "Metric.h"
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <thread>
#include <atomic>
#include <mutex>
#include <map>

class MetricsCollector {
public:
    MetricsCollector(const std::string& filename, std::chrono::seconds interval);
    ~MetricsCollector();

    template <typename T, typename... Args>
    std::shared_ptr<T> createAndRegisterMetric(const std::string& name, Args&&... args) {
        std::lock_guard<std::mutex> lock(metrics_mutex_);
        auto metric = std::make_shared<T>(name, std::forward<Args>(args)...);
        metrics_[name] = metric;
        return metric;
    }

    void start();
    void stop();

private:
    void collectionLoop();
    std::string getCurrentTimestamp();

    std::string filename_;
    std::chrono::seconds interval_;
    std::ofstream outfile_;
    std::thread collection_thread_;
    std::atomic<bool> running_;
    std::mutex metrics_mutex_;
    std::map<std::string, std::shared_ptr<Metric>> metrics_;
};

#endif // METRICS_COLLECTOR_H 