#ifndef UTILS_H
#define UTILS_H

#include "cJSON.h"

void serialize_json(const char* json_data);
void deserialize_json(const cJSON* root);
void query_json(const cJSON* root, const char* key, const char* query);
void store_json(cJSON* root, const char* key, const char* json_data);
void erase_json(cJSON* root, const char* key);
// void list_keys(const cJSON* root);
void get_serialized_keys();
void get_key(const cJSON* root, const char* key, const char* query, cJSON** result);


#endif  /* UTILS_H */