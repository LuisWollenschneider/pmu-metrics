#include "metrics.h"

// // Populated in perf_events.h
// std::map<int, std::string> event_names;

unsigned long _rdpmc(unsigned long pmc_id) {
#if _COLLECT_PMU_METRICS_
    unsigned long a, d;
    __asm__ __volatile__("rdpmc" : "=a" (a), "=d" (d) : "c" (pmc_id));
    return ((unsigned long)a) | (((unsigned long)d) << 32);
#else
    return 0;
#endif
}

unsigned long * Metrics::pmu_ids = NULL;
unsigned long * Metrics::event_ids = NULL;

std::map<unsigned long, std::string> event_names_map_copy(event_names_map, event_names_map + num_events);

Metrics::Metrics() {
    n =  3 * FIXED_CTRS_ENBL + N_PERFEVTSEL_MSR;
    metrics = (unsigned long*)malloc(sizeof(unsigned long)*n);
    if (!pmu_ids) {
        // Initialize PMU ids
        pmu_ids = (unsigned long*)malloc(sizeof(unsigned long)*n);
        int idx = 0;
        if (FIXED_CTRS_ENBL) {
            pmu_ids[0] = (1<<30);
            pmu_ids[1] = (1<<30) + 1;
            pmu_ids[2] = (1<<30) + 2;
            idx = 3;
        }
        for (int i=0; i<N_PERFEVTSEL_MSR; i++,idx++) {
            pmu_ids[idx] = i;
        }

        // Initialize Event IDs
        event_ids = (unsigned long*)malloc(sizeof(unsigned long)*n);
        idx = 0;
        if (FIXED_CTRS_ENBL) {
            event_ids[0] = ARCH_INST_RETD;
            event_ids[1] = ARCH_CORE_CYCLES;
            event_ids[2] = ARCH_TSC_REF_CYCLES;
            idx = 3;
        }
#if PERFEVTSEL0
        event_ids[idx++] = PERFEVTSEL0;
#endif
#if PERFEVTSEL1
        event_ids[idx++] = PERFEVTSEL1;
#endif
#if PERFEVTSEL2
        event_ids[idx++] = PERFEVTSEL2;
#endif
#if PERFEVTSEL3
        event_ids[idx++] = PERFEVTSEL3;
#endif
#if PERFEVTSEL4
        event_ids[idx++] = PERFEVTSEL4;
#endif
#if PERFEVTSEL5
        event_ids[idx++] = PERFEVTSEL5;
#endif
#if PERFEVTSEL6
        event_ids[idx++] = PERFEVTSEL6;
#endif
#if PERFEVTSEL7
        event_ids[idx++] = PERFEVTSEL7;
#endif
    }
}

void getMetricsStart(Metrics &m) {
    for (int i=0; i<m.n; i++) {
        m.metrics[i] = _rdpmc(m.pmu_ids[i]);
    }
}

void getMetricsEnd(Metrics &m) {
    for (int i=0; i<m.n; i++) {
        m.metrics[i] = _rdpmc(m.pmu_ids[i]) - m.metrics[i];
    }
}

void printMetrics(Metrics &m) { 
    unsigned long event_id;
    std::string event_name;
    std::cout
        << left
        << setw(40)
        << "Performance Event"
        << left
        << setw(25)
        << "Count"
        << std::endl;
    for (int i=0; i<m.n; i++) {
        event_id = m.event_ids[i];
        event_name = event_names_map_copy[event_id];
        std::cout
            << left
            << setw(40)
            << event_name
            << left
            << setw(25)
            << m.metrics[i]
            << std::endl;
    }
}
