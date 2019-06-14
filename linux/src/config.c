#include <stdio.h>
#include <string.h>
#include "config.h"

#define CONFIG_LINE_MAX_LEN 256

static const char* config_string_keys[] = {
	"secret",
	"nonce",
};

#define CONFIG_NUM_KEYS (sizeof(config_string_keys)/sizeof(const char *))

static char config_string_values[CONFIG_NUM_KEYS][CONFIG_LINE_MAX_LEN] = {{ 0 }};


char* config_get_value(const char *key)
{
	int i = 0;
	for (i=0; i<CONFIG_NUM_KEYS; i++) {
		if (strcmp(config_string_keys[i], key) == 0) {
			return config_string_values[i];
		}
	}
	fprintf(stderr, "[-] Key '%s' does not exist\n", key);
	return NULL;
}


void config_set_value(const char *key, const char *value)
{
	int i = 0;
	for (i=0; i<CONFIG_NUM_KEYS; i++) {
		if (strcmp(config_string_keys[i], key) == 0) {
			strcpy(config_string_values[i], value);
			return;
		}
	}
	fprintf(stderr, "[-] Key '%s' does not exist\n", key);
}


static char* config_string_strip(char *str)
{
	const size_t str_len = strlen(str);
	char *new_start = str;
	// Strip front
	int i = 0;
	for (i=0; i<str_len; i++) {
		if (str[i] != ' ' && str[i] != '\t') {
			break;
		}
	}
	new_start = &str[i];
	// strip back
	for (i=str_len-1; i>=new_start-str; i--) {
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\0' &&
				str[i] != '\r' && str[i] != '\n') {
			break;
		}
		str[i] = '\0';
	}
	return new_start;
}


static void config_parse_line(char *line)
{
	if (line == NULL) {
		fprintf(stderr, "[-] NULL pointer passed to config_parse_line\n");
		return;
	}
	const size_t line_len = strlen(line);
	char *separator_p = strstr(line, "=");
	if (separator_p == NULL) {
		// Not a blank line or '#' denoting a comment
		if (line_len > 1 && line[0] != '#') {
			fprintf(stderr, "[-] Failed to parse configuration line (no separator found)\n");
			fprintf(stderr, "[-] Configuration Line: %.*s\n", CONFIG_LINE_MAX_LEN, line);
		}
		return;
	}

	const size_t key_len = separator_p - line;
	if (line_len-2 <= key_len) {
		fprintf(stderr, "[*] No value provided for configuration key\n");
		fprintf(stderr, "[*] Configuration Line: %.*s\n", CONFIG_LINE_MAX_LEN, line);
		return;
	}

	*separator_p = '\0';

	char *value = config_string_strip(&separator_p[1]);
	char *key = config_string_strip(line);

	// lowercase the key
	int i = 0;
	for (i=0; i<key_len; i++) {
		if (key[i] == '\0') {
			break;
		}
		if (key[i] > 'Z' || key[i] < 'A') {
			continue;
		}
		key[i] += 0x20; // tolower
	}

	config_set_value(key, value);
}


int config_parse_file(const char *file_name)
{
	FILE *f = fopen(file_name, "r");
	if (f == NULL) {
		fprintf(stderr, "[-] Could not open configuration file '%s'\n", file_name);
		return 0;
	}

	char current_line[CONFIG_LINE_MAX_LEN] = { 0 };
	char *p = fgets(current_line, CONFIG_LINE_MAX_LEN, f);
	while (p != NULL) {
		config_parse_line(current_line);
		p = fgets(current_line, CONFIG_LINE_MAX_LEN, f);
	}

	fclose(f);
	return 1;
}


void config_dump()
{
	fprintf(stderr, "[>] Configuration values:\n");
	int i = 0;
	for (i=0; i<CONFIG_NUM_KEYS; i++) {
		fprintf(stderr, "[>] '%s': '%.*s'\n", config_string_keys[i],
				CONFIG_LINE_MAX_LEN, config_string_values[i]);
	}
}


