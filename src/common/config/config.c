#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "config.h"



const char* config_string_keys[] = {
    "secret",
    "nonce",
    "mode",
    "pausekeycode"
};

#define CONFIG_NUM_KEYS (sizeof(config_string_keys)/sizeof(const char *))

char config_string_values[CONFIG_NUM_KEYS][CONFIG_LINE_MAX_LEN] = {{ 0 }};


static int config_find_key_offset(size_t *offset, const char *key)
{
    int i = 0;
    for (i=0; i<CONFIG_NUM_KEYS; i++) {
        if (strcmp(key, config_string_keys[i]) == 0) {
            *offset = i;
            return 1; // Found match
        }
    }
    return 0;
}


// key should be lowercase
const char* config_get_value(const char *key)
{
    size_t i = 0;
    if (config_find_key_offset(&i, key) == 0) {
        fprintf(stderr, "[-] Key '%s' does not exist\n", key);
        return NULL;
    }
    return config_string_values[i];
}


// key should be lowercase
void config_set_value(const char *key, const char *value)
{
    size_t i = 0;
    if (config_find_key_offset(&i, key) == 0) {
        fprintf(stderr, "[-] Key '%s' does not exist\n", key);
        return;
    }
#ifdef _WIN32
    strcpy_s(config_string_values[i], CONFIG_LINE_MAX_LEN, value);
#else
    strncpy(config_string_values[i], value, CONFIG_LINE_MAX_LEN);
#endif
}


static char* config_string_strip(char *str)
{
    int i = 0;
    for (i=0; isspace(str[i]); i++); // Strip front
    char *ret = &str[i];
    for (; str[i]; i++);
    if (i <= 0) {
        return ret;
    }
    for (i-=1; isspace(str[i]); i--); // Strip back
    str[i+1] = '\0';
    return ret;
}


// This function will modify `line` contents as well as where `line` points to
static void config_parse_line(char *line)
{
    line = config_string_strip(line);
    char *separator_p = strstr(line, "=");
    if (separator_p == NULL) {
        // Not a blank line or '#' denoting a comment
        if (line[0] != '\0' && line[0] != '#') {
            fprintf(stderr, "[-] Invalid configuration line (no '=' found)\n" \
                    "[-] Line: %.*s\n", CONFIG_LINE_MAX_LEN, line);
        }
        return;
    }
    // Split the line into two strings and strip the two strings
    *separator_p = '\0';
    char *key = config_string_strip(line);
    char *value = config_string_strip(&separator_p[1]);

    // lowercase the key
    int i = 0;
    for (i=0; key[i]!='\0'; i++) {
        key[i] = tolower(key[i]);
    }

    config_set_value(key, value);
}


int config_parse_file(const char *filename)
{
    char line[CONFIG_LINE_MAX_LEN] = { 0 };
    FILE *f = NULL;
#ifdef _WIN32
    fopen_s(&f, filename, "r");
#else
    f = fopen(filename, "r");
#endif
    if (f == NULL) {
        fprintf(stderr, "[-] Failed to open config file\n");
        return 0;
    }

    char *p = fgets(line, CONFIG_LINE_MAX_LEN, f);
    while (p != NULL) {
        config_parse_line(line);
        p = fgets(line, CONFIG_LINE_MAX_LEN, f);
    }

    fclose(f);
    return 1;
}


void config_dump()
{
    fprintf(stderr, "[>] Configuration values:\n");
    int i = 0;
    for (i=0; i<CONFIG_NUM_KEYS; i++) {
        fprintf(stderr, "[>] '%s': '%s'\n", config_string_keys[i],
                config_string_values[i]);
    }
}
