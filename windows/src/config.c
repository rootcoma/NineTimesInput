#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include "config.h"


// Values stored for configuration
static char config_string_values[CONFIG_NUM_KEYS][CONFIG_LINE_MAX_LEN] = {{ 0 }};


// key should be lowercase
char* config_get_value(const char *key)
{
    int i = 0;
    for (i=0; i<CONFIG_NUM_KEYS; i++) {
        if (strcmp(key, config_string_keys[i]) == 0) {
            return config_string_values[i]; // Found match
        }
    }
    fprintf(stderr, "[-] Key '%s' does not exist\n", key);
    return NULL;
}


// key should be lowercase
void config_set_value(const char *key, const char *value)
{
    int i = 0;
    for (i=0; i<CONFIG_NUM_KEYS; i++) {
        if (strcmp(key, config_string_keys[i]) == 0) {
            strcpy_s(config_string_values[i], CONFIG_LINE_MAX_LEN, value);
            return; // Found match
        }
    }
    fprintf(stderr, "[-] Key '%s' does not exist\n", key);
}


// This modifies `line`
static void config_parse_line(char *line)
{
    if (line == NULL) {
        fprintf(stderr, "[!] NULL pointer passed to configs parse_line\n");
        return;
    }
    const size_t line_len = strlen(line);
    char key[CONFIG_LINE_MAX_LEN] = { 0 };
    char value[CONFIG_LINE_MAX_LEN] = { 0 };
    char *separator_p = strstr(line, "=");
    if (separator_p == NULL) {
        // TODO: Check if line is just whitespace
        // TODO: Allow for comment if # is not first char?
        if (line_len > 1 && line[0] != '#') {
            fprintf(stderr, "[-] Failed to parse config line.\n");
            fprintf(stderr, "[-] Config line: %s\n", line);
        }
        return;
    }

    const size_t key_len = separator_p-line;
    // ensure that there is a character after the '=' separator
    // Subtract 2 for newline and equals
    if (line_len-2 <= key_len) {
        fprintf(stderr, "[*] No value provided for key.\n");
        fprintf(stderr, "[*] Config line: %s\n", line);
        return;
    }

    // Split string into two
    *separator_p = '\0';

    // read the key, line points to it, the separator is now null terminated
    int num_read = sscanf_s(line, " %s ", key);
    if (num_read == 0) {
        fprintf(stderr, "[-] Error reading line key\n");
        fprintf(stderr, "[-] Config line: %s\n", line);
        return;
    }

    // Convert the key to lowercase
    int i = 0;
    for (i=0; i<CONFIG_LINE_MAX_LEN && key[i]!='\0'; i++) {
        if (key[i] < 'A' || key[i] > 'Z') {
            continue;
        }
        key[i] += 0x20; // 0x41: A, 0x61: a
    }
    //fprintf(stderr, "CONFIG_KEY: %s\n", key);

    // read the value, separator_p + 1 is the first char of the value
    // %[ -~] is ascii 0x20-0x7E, most printable chars excluding newlines
    num_read = sscanf_s(&separator_p[1], " %[ -~] ", value);
    if (num_read == 0) {
        fprintf(stderr, "[-] Error reading line value\n");
        fprintf(stderr, "[-] Config line: %s\n", line);
        return;
    }
    
    // Strip trailing spaces from value
    i=CONFIG_LINE_MAX_LEN-1;
    for (; i>=0 && (value[i]==' ' || value[i]=='\0'); i--) {
        value[i] = '\0';
    }
    //fprintf(stderr, "CONFIG_VALUE: %s\n", value);

    config_set_value(key, value);
}


int config_parse_file(const char *filename)
{
    char line[CONFIG_LINE_MAX_LEN] = { 0 }; // Hold this, stack!
    FILE *f = NULL;
    fopen_s(&f, filename, "r");
    if (f == NULL || feof(f)) {
        fprintf(stderr, "[-] Failed to open config file\n");
        return -1;
    }

    char *p = fgets(line, CONFIG_LINE_MAX_LEN, f);
    while (p != NULL) {
        //fprintf(stderr, "[*] Parsing line %s", line);
        config_parse_line(line);
        p = fgets(line, CONFIG_LINE_MAX_LEN, f);
    }

    fclose(f);
    return 0;
}


void config_dump()
{
    fprintf(stderr, "[>] Configuration values:\n");
    int i = 0;
    for(i=0; i<CONFIG_NUM_KEYS; i++) {
        fprintf(stderr, "[>] '%s': '%s'\n", config_string_keys[i],
                config_string_values[i]);
    }
}

