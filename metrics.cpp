#include "metrics.h"

unsigned long _rdpmc(unsigned long pmc_id) {
#if _COLLECT_PMU_METRICS_
    unsigned long a, d;
    __asm__ __volatile__("rdpmc" : "=a" (a), "=d" (d) : "c" (pmc_id));
    return ((unsigned long)a) | (((unsigned long)d) << 32);
#else
    return 0;
#endif
}

unsigned long getTimeDiff(struct timespec start_time, struct timespec end_time) {
    return (unsigned long)((end_time.tv_sec - start_time.tv_sec)*1000000000 +
        double(end_time.tv_nsec - start_time.tv_nsec));
    return 0;
}


unsigned long getMetricFromName(std::string event_name) {
    for (int i=0; i<num_events; i++) {
        if (std::get<1>(event_names_map[i]) == event_name) {
            return std::get<0>(event_names_map[i]);
        }
    }

    std::cerr << "Event name '" << event_name << "' not found" << std::endl;
    exit(1);
}


unsigned long * Metrics::pmu_ids = NULL;
unsigned long * Metrics::event_ids = NULL;

bool metrics_in_use = false;

Metrics::Metrics(std::vector<std::string> event_names) {
    if (event_names.size() > 8) {
        std::cerr << "Number of events exceeds the maximum limit of 8" << std::endl;
        exit(1);
    }

    if (metrics_in_use) {
        std::cerr << "Only one instance of Metrics can be used at a time" << std::endl;
        exit(1);
    }

    metrics_in_use = true;

    n = event_names.size();
    metrics = (unsigned long*)malloc(sizeof(unsigned long)*n);
    names = (std::string*)malloc(sizeof(std::string)*n);

    // Initialize PMU ids
    pmu_ids = (unsigned long*)malloc(sizeof(unsigned long)*n);
    int idx = 0;
    for (int i=0; i<event_names.size(); i++,idx++) {
        pmu_ids[idx] = i;
    }

    // Initialize Event IDs
    event_ids = (unsigned long*)malloc(sizeof(unsigned long)*n);
    idx = 0;

    for (int i=0; i<event_names.size(); i++) {
        event_ids[idx++] = getMetricFromName(event_names[i]);
        names[i] = event_names[i];
    }
}

Metrics::~Metrics() {
    free(metrics);
    free(names);
    free(pmu_ids);
    metrics_in_use = false;
}

void getMetricsStart(Metrics &m) {
    clock_gettime(CLOCK_MONOTONIC, &m.startTime);
    for (int i=0; i < m.n; i++) {
        m.metrics[i] = _rdpmc(m.pmu_ids[i]);
    }
}

void getMetricsEnd(Metrics &m) {
    for (int i=0; i < m.n; i++) {
        m.metrics[i] = _rdpmc(m.pmu_ids[i]) - m.metrics[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &m.endTime);
    m.timeElapsedns = getTimeDiff(m.startTime, m.endTime);
}

void printMetrics(Metrics &m) { 
    unsigned long event_id;
    std::string event_name;
    std::cout << "Total time elapsed (ns): " << m.timeElapsedns << std::endl;
    std::cout << "--" << std::endl;
    std::cout
        << std::left << std::setw(40) << "Performance Event"
        << std::left << std::setw(25) << "Count"
        << std::endl;
    for (int i=0; i < m.n; i++) {
        event_id = m.event_ids[i];
        event_name = m.names[i];
        std::cout 
            << std::left << std::setw(40)<< event_name
            << std::left << std::setw(25) << m.metrics[i]
            << std::endl;
    }
}
