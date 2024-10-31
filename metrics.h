#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <vector>

#ifndef _METRICS_H_
#define _METRICS_H_

#include "perf_events.h"

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
    unsigned long *pmu_ids;
    unsigned long *event_ids;
    std::vector<std::string> names;
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

Metrics m = Metrics({<...event names...>});
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
