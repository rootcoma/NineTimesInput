#ifndef KXM_CONFIG_H
#define KXM_CONFIG_H

#define CONFIG_LINE_MAX_LEN 256

static const char* config_string_keys[] = {
    "secret",
    "nonce",
};

#define CONFIG_NUM_KEYS (sizeof(config_string_keys)/sizeof(const char*))


char* config_get_value(const char* key);

void config_set_value(const char* key, const char* value);

int config_parse_file(const char* file_name);

void config_dump();


#endif

