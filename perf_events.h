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

typedef std::tuple<unsigned long, std::string, std::string> event_name_tuple_t;

/*
 * Map of event IDs to event names.
 * (event_id, event_name, event_description)
 * 
 * event_name format: "ARCHITECTURE.EVENT_NAME"
 * 
 * Referenced from:
 * https://perfmon-events.intel.com/
 */
const event_name_tuple_t event_names_map[] = {
	event_name_tuple_t(0xC204, "SKYLAKE.UOPS_RETIRED.MACRO_FUSED", "Uops Retired: Macro-fused")
};

const int num_events = sizeof(event_names_map) / sizeof(event_name_tuple_t);

#endif // __PERF_EVENTS__
