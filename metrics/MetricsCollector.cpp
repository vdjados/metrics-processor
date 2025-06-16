#include "MetricsCollector.h"
#include <iostream>

MetricsCollector::MetricsCollector(const std::string& filename, std::chrono::seconds interval)
    : filename_(filename),
      interval_(interval),
      running_(false) {
    outfile_.open(filename_, std::ios::app);
    if (!outfile_.is_open()) {
        std::cerr << "Error opening metrics file: " << filename_ << std::endl;
    }
}

MetricsCollector::~MetricsCollector() {
    stop();
    if (outfile_.is_open()) {
        outfile_.close();
    }
}

void MetricsCollector::start() {
    if (!running_) {
        running_ = true;
        collection_thread_ = std::thread(&MetricsCollector::collectionLoop, this);
    }
}

void MetricsCollector::stop() {
    if (running_) {
        running_ = false;
        if (collection_thread_.joinable()) {
            collection_thread_.join();
        }
    }
}

void MetricsCollector::collectionLoop() {
    while (running_) {
        std::this_thread::sleep_for(interval_);

        std::lock_guard<std::mutex> lock(metrics_mutex_);
        if (outfile_.is_open()) {
            std::string timestamp = getCurrentTimestamp();
            for (const auto& pair : metrics_) {
                outfile_ << timestamp << " \"" << pair.second->getName() << "\" " << pair.second->getValueAsStringAndReset() << std::endl;
            }
            outfile_.flush();
        } else {
            std::cerr << "Metrics file not open!" << std::endl;
        }
    }
}

std::string MetricsCollector::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    ss << "." << std::setw(3) << std::setfill('0') << ms.count();

    return ss.str();
} 