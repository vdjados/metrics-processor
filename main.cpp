#include "metrics/MetricsCollector.h"
#include "metrics/CounterMetric.h"
#include "metrics/GaugeMetric.h"
#include <iostream>
#include <vector>
#include <thread>
#include <random>

void simulate_work(std::shared_ptr<CounterMetric> http_requests, std::shared_ptr<GaugeMetric> cpu_utilization) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_requests(1, 10);
    std::uniform_real_distribution<> dist_cpu(0.0, 2.0);

    for (int i = 0; i < 20; ++i) {
        http_requests->record(static_cast<long long>(dist_requests(gen)));
        cpu_utilization->record(dist_cpu(gen));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    MetricsCollector collector("metrics.log", std::chrono::seconds(1));

    auto http_requests = collector.createAndRegisterMetric<CounterMetric>("HTTP requests RPS");
    auto cpu_utilization = collector.createAndRegisterMetric<GaugeMetric>("CPU");

    collector.start();

    std::vector<std::thread> workers;
    for (int i = 0; i < 5; ++i) {
        workers.emplace_back(simulate_work, http_requests, cpu_utilization);
    }

    for (auto& worker : workers) {
        worker.join();
    }

    std::cout << "Simulated work finished. Collecting metrics for a few more seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    collector.stop();

    std::cout << "Metrics collection stopped. Check metrics.log for output." << std::endl;

    return 0;
} 