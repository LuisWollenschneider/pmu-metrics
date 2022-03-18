# Fine-grained Hardware Metrics using Intel's PMU

Intel's Performance Monitoring Unit (PMU) is a convenient way to obtain hardware metrics such as cache misses, core cycles, TLB misses, etc. in a lightweight non-intrusive manner. A useful application of PMU metrics is when one wants to benchmark a subsection of code, which is difficult to do with existing tools such as linux_perf, Vtune, etc.

## Setup

1. Check your Intel CPU's microarchitecture. On Linux systems, this information can be ontained as follows:
```
$ cat /sys/devices/cpu/caps/pmu_name
skylake 
```
For instance, the Intel Xeon Silver 4114 CPU belongs to the skylake microarchitecture family.

2. Choose performance events from file `perf_events.h` and set the `PERFEVTSELx` headers in file `config.h`. Events starting with prefix `ARCH_` work on all Intel microarchitectures, whereas architecture-specific events start with the approrpiate prefix:
```
#define PERFEVTSEL0	ARCH_LLC_MISS
#define PERFEVTSEL1	SKL_KBL_L2_MISS
#define PERFEVTSEL2	SKL_KBL_L2_REFS
```
Above we have chosen to track last-level cache (typically L3) misses which is an architectural performance event, and L2 misses and L1 misses (L2 references) for the skylake and kaby lake microarchitectures.

After choosing performance events, set the header `N_PERFEVTSEL_MSR` to number of events being counted:
```
#define N_PERFEVTSEL_MSR 3
```
You can track events up to the available number of PERFEVTSEL registers provided by your CPU (typically 4, but can be 8 in some cases).

3. Run the `setup.sh` script to program the PMU on the chosen CPU core
```
$ sudo ./setup.sh -c 1
```
`-c` flag specifies the logical core to program. If unspecified, core 1 is programmed by default.

## Usage

Wrap the code you want to benchmark with the helper functions `getMetricsStart`, `getMetricsEnd`, and optionally, `printMetrics`. Here is an example:
```
#include <vector>
#include <algorithm>

#include "metrics.h"

#define N 10000000 // 10^7

using namespace std;

int main() {
	Metrics m;

	/* Initialize the vector with random numbers */
	vector<int> vect(N);
	for (int i=0; i<N; i++) {
		vect[i] = rand();
	}

	getMetricsStart(m);
	/******** Code to benchmark ********/
	sort(vect.begin(), vect.end());
	/***********************************/
	getMetricsEnd(m);
	printMetrics(m);

	return 0;
}
```

**IMPORTANT** - Remember to taskset the process on the programmed core:
```
$ taskset -c 1 ./benchmark.out
```

The program yields the following metrics:
```
$ taskset -c 1 ./benchmark.out
Performance Event            Count
Instructions Retired         24400763236
Core Cycles                  14213176160
TSC Reference Cycles         11386341560
LLC (L3) Misses              2524985
L2 Misses                    5542342
L1 Misses (L2 References)    9631636
```

## Extending the Library

More performance events can readily be added to the library by declaring them in the file `perf_events.h`. You will need to obtain the eventID and unit mask of the performance event from Chapter 19 of the [Intel Developer's Manual Vol. 3](https://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-system-programming-manual-325384.html).