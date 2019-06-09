#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include "keyboardhook.h"

#define DEVICE_FILE_NAME "/proc/bus/input/devices"

static const char* mouse_device_evs[] = {
		"b",
		"B",
};

// returns event id
int find_first_mouse()
{
	int keyboard_id = -1;
	FILE *f = fopen(DEVICE_FILE_NAME, "r");
	if (f == NULL) {
		fprintf(stderr, "[-] Failed to open proc devices");
		return keyboard_id;
	}
#define BUFFER_SIZE 256
	char *p;
	char buf[BUFFER_SIZE] = { 0 };
	char *match;
	int last_event_device_id;
	int num_matches = 0;
	p = fgets(buf, BUFFER_SIZE, f);
	while (p != NULL) {
		//match = strstr(p, "mouse");
		match = strstr(p, "event");
		if (match != NULL) {
			//sscanf(match, " mouse%i", &last_event_device_id);
			sscanf(match, " event%i", &last_event_device_id);
		} else if (strstr(p, "B: EV=") != NULL) {
			num_matches = sscanf(p + sizeof(char)*3, "EV=%s", buf);
			if (num_matches <= 0) {
    			continue; // not what we need
			}
			int i = 0;
			int found = 0;
			for(i=0;i<sizeof(mouse_device_evs)/sizeof(char *);i++) {
				if (!strcmp(mouse_device_evs[i], buf)) {
					found = 1;
				}
			}
			if (found) {
				break;
			}
		}	
		p = fgets(buf, BUFFER_SIZE, f);
	}
	fclose(f);

	return last_event_device_id;
}


