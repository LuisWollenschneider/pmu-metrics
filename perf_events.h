/***********************************************************
* Names/short descriptions of performance events present
* in the library. These names are used in the printMetrics
* function.
*
* IMPORTANT: Remember to add the name of the event to the
* event_names_map in this file for newly added events to
* perf_events.h
***********************************************************/

#ifndef __PERF_EVENTS__
#define __PERF_EVENTS__

#include <map>
#include <string>

#include "perf_events.h"

/*
 * Microcode & Performance Counter Event mapping
 *
 * EventName -> MicroCode -> EventID
 * 
 * EventID: (EventSel << 8) | Umask
 * 
 * Referenced from:
 * https://perfmon-events.intel.com/
*/
const std::map<std::string, std::map<std::string, int>> event_microcode_map = {
	{"MACHINE_CLEARS.COUNT", {
		{"ICE_LAKE", 0xC301},
		{"RAPTOR_LAKE", 0xC301},
		{"ADLER_LAKE", 0xC301},
		{"EMERALD_RAPIDS", 0xC301}
	}},
	{"MEM_LOAD_RETIRED.L1_HIT", {
		{"SKYLAKE", 0xD101}
	}},
	{"UOPS_ISSUED.ANY", {
		{"ICE_LAKE", 0x0E01},
		{"TREMONT", 0x0E00},
		{"EMERALD_RAPIDS", 0xAE01}
	}},
	{"UOPS_RETIRED.ALL", {
		{"RAPTOR_LAKE", 0xC200},
		{"ADLER_LAKE", 0xC200},
		{"TREMONT", 0xC200}
	}},
	{"UOPS_RETIRED.MACRO_FUSED", {
		{"SKYLAKE", 0xC204}
	}},
	{"UOPS_RETIRED.RETIRE_SLOTS", {
		{"SKYLAKE", 0xC202}
	}},
};

#endif // __PERF_EVENTS__
