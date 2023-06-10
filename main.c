#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Error: Insufficient arguments.\n");
        printf("Usage: %s <operation> [<JSON data>]\n", argv[0]);
        printf("Available operations: serialize, deserialize, store, erase, keys, query\n");
        return 1;
    }

    const char* operation = argv[1];
    const char* json_data = argc >= 3 ? argv[2] : NULL;

    if (strcmp(operation, "serialize") == 0) {
        serialize_json(json_data);
    } else if (strcmp(operation, "deserialize") == 0) {
        deserialize_json(json_data);
    } else if (strcmp(operation, "store") == 0) {
        if (argc < 4) {
            printf("Error: Key argument is missing for the 'store' operation.\n");
            return 1;
        }
        const char* key = argv[3];
        store_json(key, json_data);
    } else if (strcmp(operation, "erase") == 0) {
        if (argc < 3) {
            printf("Error: Key argument is missing for the 'erase' operation.\n");
            return 1;
        }
        const char* key = argv[2];
        erase_json(key);
    } else if (strcmp(operation, "keys") == 0) {
        get_serialized_keys();
    } else if (strcmp(operation, "query") == 0) {
        if (argc < 3) {
            printf("Error: Key argument is missing for the 'query' operation.\n");
            return 1;
        }
        const char* key = argv[2];
        query_json(key);
    } else {
        printf("Error: Invalid operation.\n");
        return 1;
    }

    return 0;
}