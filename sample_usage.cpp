/******************************************************************
* Sample usage of the pmu-metrics library.
* IMPORTANT: Remember to taskset to the programmed core as follows:

node:~> taskset -c 1 ./benchmark.out
*******************************************************************/

#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

#include "metrics.h"

#define N 1000000 // 1M

using namespace std;

void runTest() {
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
}

int main() {
	std::cout << "[*] Running sort on " << N << " *RANDOM* elements" << std::endl;
	runTest();

	std::cout << "[*] Running sort on " << N << " *STATIC* elements" << std::endl;
	std::srand(0);
	runTest();

	return 0;
}
