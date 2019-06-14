#ifndef KXM_CONFIG_H
#define KXM_CONFIG_H


char* config_get_value(const char *key);


void config_set_value(const char *key, const char *value);


int config_parse_file(const char *file_name);


void config_dump();


#endif


