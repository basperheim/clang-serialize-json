#ifndef UTILS_H
#define UTILS_H

#include "cJSON.h"

cJSON* load_serial_file();
void serialize_json(const cJSON* root);
void deserialize_json(const cJSON* root);
void query_json(const cJSON* root, const char* key);
void store_json(cJSON* root, const char* key, const cJSON* json_data);
void erase_json(cJSON* root, const char* key);
void get_serialized_keys();

#endif /* UTILS_H */