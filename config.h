/***********************************************************
* Config file for setting up pmu-metrics library.
* Specify what performance events to count here.
* For a (non-exhaustive) list of performance events, refer
* to file perf_events.h.
***********************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "perf_events.h"

/*************************************************
* Enable fixed counters if they are available,
* set to 0 otherwise.
* PMU version 2 and above generally have fixed
* counters. This information can be obtained from
* CPUID instruction 0xAH leaf. See table 19-2 in
* the Intel Developer's Manual Vol. 3B, or refer
* to the description in perf_events.h.
**************************************************/
#define FIXED_CTRS_ENBL		0

/**********************************************
* Number of programmable PERFEVTSEL MSRs to use.
* There can be 4-8 MSRs, typically at least 4.
* This information can be obtained from CPUID
* instruction 0xAH leaf.
***********************************************/
#define N_PERFEVTSEL_MSR	3

/***********************************************************************
* Choose performance events to count. You can choose up to the
* available number of PERFEVTSEL MSRs (setting N_PERFEVTSEL_MSR above).
*
* Note that seven architectural performance events are available on all
* Intel PMU. See perf_events.h, first three events are fixed counters.
*
* For other performance events, you need to know the architecture
* family of your processor. You can obtain this information as follows:

node:~> cat /sys/devices/cpu/caps/pmu_name
skylake

* Depending on the architecture family, you can choose from the diverse
* set of available non-architectural events. Refer to Chapter 19 of the
* Intel Developer's Manual Vol. 3B for available events for your CPU's
* architecture family. For Skylake and Kaby Lake microarchitectures,
* a select few events have been included in perf_events.h. A full list of
* events for Skylake and Kaby Lake are present in Table 19-3 of the
* Intel Developer's Manual Vol. 3B.
*
* For instance, on a Skylake architecture, we've chosen the following
* three performance events:
- L3 Misses (architectural event)
- L2 Misses (non-architectural event, specific to Skylake and Kaby Lake)
- L1 Misses (L2 References, non-architectural event)
************************************************************************/
#define PERFEVTSEL0	ARCH_LLC_MISS
#define PERFEVTSEL1	SKL_KBL_L2_MISS
#define PERFEVTSEL2	SKL_KBL_L2_REFS
// #define PERFEVTSEL3	NULL
// #define PERFEVTSEL4	NULL
// #define PERFEVTSEL5	NULL
// #define PERFEVTSEL6	NULL
// #define PERFEVTSEL7	NULL

#endif // __CONFIG_H__