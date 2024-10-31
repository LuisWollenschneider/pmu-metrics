#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <vector>

#ifndef _METRICS_H_
#define _METRICS_H_

/*************************************************
* Enable fixed counters if they are available,
* set to 0 otherwise.
* PMU version 2 and above generally have fixed
* counters. This information can be obtained from
* CPUID instruction 0xAH leaf. See table 19-2 in
* the Intel Developer's Manual Vol. 3B, or refer
* to the description in perf_events.h.
**************************************************/
#define FIXED_CTRS_ENBL		1


#include "perf_events.h"

/***********************************************************
* Flag to enable collection of hardware metrics.
* Make sure the setup script has been run successfully
* before enabling this flag.
***********************************************************/
#define _COLLECT_PMU_METRICS_ 1

class Metrics {
    public:
	Metrics(std::vector<std::string>);

    ~Metrics();

	/* Measure time elapsed */
	struct timespec startTime;
    struct timespec endTime;
    unsigned long timeElapsedns;

	int n; // number of metrics being measured
    unsigned long *metrics;
    std::string *names;
    static unsigned long *pmu_ids;
    static unsigned long *event_ids;
};

/***********************************************************
* Function to read HW counters
***********************************************************/

unsigned long _rdpmc(unsigned long pmc_id);
unsigned long getTimeDiff(struct timespec, struct  timespec);

/************************************************************
* USAGE - To collect metrics for a particular section of code,
* wrap it with getMetricsStart() and getMetricsEnd()

<...code...>

Metrics m;
getMetricsStart(m);
<...
code snippet to monitor
...>
getMetricsEnd(m);

<...code...>

************************************************************/

void getMetricsStart(Metrics&);
void getMetricsEnd(Metrics&);
void printMetrics(Metrics&);

#endif // _METRICS_H_
