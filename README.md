Fork from [UWHustle/pmu-metrics](https://github.com/UWHustle/pmu-metrics)

# Fine-grained Hardware Metrics using Intel's PMU

Intel's Performance Monitoring Unit (PMU) is a convenient way to obtain hardware metrics such as cache misses, core cycles, TLB misses, etc. in a lightweight non-intrusive manner. A useful application of PMU metrics is when one wants to benchmark a subsection of code, which is difficult to do with existing tools such as linux_perf, Vtune, etc.

## Setup

1. Check your Intel CPU's microarchitecture. On Linux systems, this information can be ontained as follows:
```
$ cat /sys/devices/cpu/caps/pmu_name
skylake 
```
For instance, the Intel Xeon Silver 4114 CPU belongs to the skylake microarchitecture family.

2. Choose performance events from file `perf_events.h`.

Use the name in the format `"ARCHITECTURE.EVENT_NAME"` to specify the performance event when initializing the `Metrics` object.

3. Run the `setup.sh` script to program the PMU on the chosen CPU core
```
$ sudo ./setup.sh -c 1
```
`-c` flag specifies the logical core to program. If unspecified, core 1 is programmed by default.

## Usage

Initialize the `Metrics` object with the performance events you want to track.

Format: `"ARCHITECTURE.EVENT_NAME"`

Wrap the code you want to benchmark with the helper functions `getMetricsStart`, `getMetricsEnd`, and optionally, `printMetrics`. Here is an example:
```cpp
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

#include "metrics.h"

#define N 10000000 // 10M

using namespace std;

int main() {
	vector<int> vect(N);

	Metrics m = Metrics({"SKYLAKE.UOPS_RETIRED.MACRO_FUSED"});

	// Initialize the array with random numbers
	for (int i=0; i<N; i++) {
		vect[i] = rand();
	}

	// Sort the elements, measure metrics
	getMetricsStart(m);
	sort(vect.begin(), vect.end());
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
Total time elapsed (ns): 7348051209
--
Performance Event                       Count                    
SKYLAKE.UOPS_RETIRED.MACRO_FUSED        3722016
```

## Extending the Library

More performance events can readily be added to the library by adding them in the file `perf_events.h` to the `event_names_map` array.
You will need to obtain the eventID and unit mask of the performance event from Chapter 19 of the [Intel Developer's Manual Vol. 3](https://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-system-programming-manual-325384.html).
