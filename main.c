#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "utils.h"

#define SERIAL_FILE_PATH "serial.json"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Error: No operation specified.\n");
        printf("Available commands: serialize, deserialize, query, store, erase, keys\n");
        return 1;
    }


    // Check if the serial file exists
    FILE* file = fopen(SERIAL_FILE_PATH, "r");
    if (file == NULL) {
        // Create a new serial file with {}
        file = fopen(SERIAL_FILE_PATH, "w");
        if (file == NULL) {
            printf("Error: Failed to create the serial file.\n");
            return 1;
        }
        fputs("{}", file);
        fclose(file);
    } else {
        // Check if the serial file is empty
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        if (file_size == 0) {
            // File is empty, write {}
            fclose(file);
            file = fopen(SERIAL_FILE_PATH, "w");
            if (file == NULL) {
                printf("Error: Failed to open the serial file for writing.\n");
                return 1;
            }
            fputs("{}", file);
            fclose(file);
        } else {
            fclose(file);
        }
    }

    const char* operation = argv[1];

    if (strcmp(operation, "serialize") == 0) {
        if (argc < 3) {
            printf("Error: No JSON data provided for serialization.\n");
            return 1;
        }
        const char* json_data = argv[2];
        cJSON* root = cJSON_Parse(json_data);
        if (root == NULL) {
            printf("Error: Failed to parse JSON data.\n");
            return 1;
        }
        serialize_json(root);
        cJSON_Delete(root);
    } else if (strcmp(operation, "deserialize") == 0) {
        deserialize_json(load_serial_file());
    } else if (strcmp(operation, "query") == 0) {
        if (argc < 3) {
            printf("Error: No key provided for query operation.\n");
            return 1;
        }
        const char* key = argv[2];
        query_json(load_serial_file(), key);
    } else if (strcmp(operation, "store") == 0) {
        if (argc < 4) {
            printf("Error: Insufficient arguments for store operation.\n");
            return 1;
        }
        const char* key = argv[2];
        const char* json_data = argv[3];
        store_json(load_serial_file(), key, cJSON_Parse(json_data));
    } else if (strcmp(operation, "erase") == 0) {
        if (argc < 3) {
            printf("Error: No key provided for erase operation.\n");
            return 1;
        }
        const char* key = argv[2];
        erase_json(load_serial_file(), key);
    } else if (strcmp(operation, "keys") == 0) {
        get_serialized_keys();
    } else {
        printf("Error: Invalid operation.\n");
        return 1;
    }

    return 0;
}