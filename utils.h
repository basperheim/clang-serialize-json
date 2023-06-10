#ifndef UTILS_H
#define UTILS_H

void serialize_json(const char* json_data);
void deserialize_json(const char* serialized_data);
void store_json(const char* key, const char* json_data);
void erase_json(const char* key);
void get_serialized_keys();
void query_json(const char* key);

#endif /* UTILS_H */