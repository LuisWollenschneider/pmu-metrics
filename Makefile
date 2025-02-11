target: sample_usage.o metrics.o
	g++ -o benchmark.out sample_usage.o metrics.o -std=c++17

sample_usage.o: sample_usage.cpp
	g++ -c sample_usage.cpp -std=c++17

metrics.o: metrics.cpp metrics.h perf_events.h perf_events.h
	g++ -c metrics.cpp -std=c++17

clean:
	rm -rf *.out
	rm -rf *.o