#include "metrics.h"

unsigned long _rdpmc(unsigned long pmc_id) {
#if defined(__i386__) || defined(__x86_64__)
    unsigned long a, d;
    __asm__ __volatile__("rdpmc" : "=a" (a), "=d" (d) : "c" (pmc_id));
    return ((unsigned long)a) | (((unsigned long)d) << 32);
#else
    std::cerr << "rdpmc not supported on this architecture" << std::endl;
    exit(1);
#endif
}

unsigned long getTimeDiff(struct timespec start_time, struct timespec end_time) {
    return (unsigned long)((end_time.tv_sec - start_time.tv_sec)*1000000000 +
        double(end_time.tv_nsec - start_time.tv_nsec));
    return 0;
}


unsigned long getMetricFromName(std::string event_name) {
    // event_architecture_map get event name
    if (event_microcode_map.find(event_name) == event_microcode_map.end()) {
        std::cerr << "Event name '" << event_name << "' not found" << std::endl;
        exit(1);
    }
    std::map<std::string, int> event_map = event_microcode_map.at(event_name);

    std::string microcode = "";
    std::FILE* file = std::fopen("/sys/devices/cpu/caps/pmu_name", "r");
    if (file == NULL) {
        std::cerr << "Error opening /sys/devices/cpu/caps/pmu_name" << std::endl;
        exit(1);
    }

    char line[256];
    std::fgets(line, sizeof(line), file);
    std::fclose(file);

    // upper case
    for (int i=0; i<sizeof(line); i++) {
        if (line[i] == '\0' || line[i] == '\n') {
            break;
        }
        microcode += std::toupper(line[i]);
    }

    if (event_map.find(microcode) == event_map.end()) {
        std::cerr << "Event name '" << event_name << "' not found for microcode '" << microcode << "'" << std::endl;
        exit(1);
    }

    return event_map.at(microcode);
}

bool metrics_in_use = false;

Metrics::Metrics(std::vector<std::string> event_names) {
    n = event_names.size();

    if (n > 8) {
        std::cerr << "Number of events exceeds the maximum limit of 8" << std::endl;
        exit(1);
    }

    if (metrics_in_use) {
        std::cerr << "Only one instance of Metrics can be used at a time" << std::endl;
        exit(1);
    }

    int cpu_core = 1;

    metrics_in_use = true;

    // Initialize names
    names = event_names;
    // Initialize metrics
    metrics = (unsigned long*)malloc(sizeof(unsigned long) * n);
    // Initialize PMU ids
    pmu_ids = (unsigned long*)malloc(sizeof(unsigned long) * n);
    // Initialize Event IDs
    event_ids = (unsigned long*)malloc(sizeof(unsigned long) * n);

    std::string command = "";

    if (n > 5) {
        // wrmsr -p ${CORE} 0x38f 0x7000000ff

        command.clear();
        command.append("wrmsr -p " + std::to_string(cpu_core) + " 0x38f 0x7000000ff");

        system(command.c_str());
    } else {
        // wrmsr -p ${CORE} 0x38f 0x70000000f
        command.clear();
        command.append("wrmsr -p " + std::to_string(cpu_core) + " 0x38f 0x70000000f");

        system(command.c_str());
    }

    if (false) {
        // wrmsr -p ${CORE} 0x38d 0x333
        command.clear();
        command.append("wrmsr -p " + std::to_string(cpu_core) + " 0x38d 0x333");
        system(command.c_str());
    }

    for (int i=0; i < n; i++) {
        pmu_ids[i] = i;
        event_ids[i] = getMetricFromName(event_names[i]);

        // wrmsr -p ${CORE} 0x186 ${PERFEVTSEL0_MASK}
        command.clear();
        command.append("wrmsr -p " + std::to_string(cpu_core) + " " + std::to_string(0x186 + i) + " " + std::to_string(0x430000 + event_ids[i]));
        system(command.c_str());
    }
}

Metrics::~Metrics() {
    free(metrics);
    metrics = NULL;
    free(event_ids);
    event_ids = NULL;
    free(pmu_ids);
    pmu_ids = NULL;

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
    std::cout << "Total time elapsed (ns): " << m.timeElapsedns << std::endl;
    std::cout << "------" << std::endl;
    std::cout
        << std::left << std::setw(40) << "Performance Event"
        << std::left << std::setw(25) << "Count"
        << std::endl;
    for (int i=0; i < m.n; i++) {
        std::cout 
            << std::left << std::setw(40)<< m.names[i]
            << std::left << std::setw(25) << m.metrics[i]
            << std::endl;
    }
    std::cout << "------" << std::endl;
}
