#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h"
#include "utils.h"

#define SERIAL_FILE_PATH "serial.json"

cJSON* load_serial_file() {
    FILE* file = fopen(SERIAL_FILE_PATH, "rb");
    if (file == NULL) {
        printf("Error: Failed to open the serial file.\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* serialized_data = (char*)malloc(file_size);
    if (serialized_data == NULL) {
        printf("Error: Failed to allocate memory for serial file data.\n");
        fclose(file);
        return NULL;
    }

    fread(serialized_data, file_size, 1, file);

    fclose(file);

    cJSON* root = cJSON_Parse(serialized_data);
    free(serialized_data);

    if (root == NULL) {
        printf("Error: Failed to parse the serialized data.\n");
        return NULL;
    }

    return root;
}

void serialize_json(const cJSON* root) {
    if (root == NULL) {
        printf("Error: Invalid JSON data.\n");
        return;
    }

    FILE* file = fopen(SERIAL_FILE_PATH, "wb");
    if (file == NULL) {
        printf("Error: Failed to open the serial file for writing.\n");
        return;
    }

    char* serialized_data = cJSON_PrintUnformatted(root);
    fwrite(serialized_data, strlen(serialized_data), 1, file);

    fclose(file);
    free(serialized_data);
}

void deserialize_json(const cJSON* root) {
    if (root == NULL) {
        printf("Error: Invalid JSON data.\n");
        return;
    }

    cJSON* data = cJSON_GetObjectItem(root, "message");
    if (cJSON_IsString(data)) {
        printf("%s\n", data->valuestring);
    }
}

void query_json(const cJSON* root, const char* key) {
    if (root == NULL) {
        printf("Error: Failed to load the serial file.\n");
        return;
    }

    cJSON* json_data = cJSON_GetObjectItem(root, key);
    if (json_data == NULL) {
        printf("Key '%s' does not exist.\n", key);
        return;
    }

    printf("%s\n", cJSON_Print(json_data));
}

void store_json(cJSON* root, const char* key, const cJSON* json_data) {
    if (root == NULL) {
        printf("Error: Failed to load the serial file.\n");
        return;
    }

    if (json_data == NULL) {
        printf("Error: Invalid JSON data.\n");
        return;
    }

    // Convert the JSON data to a string
    char* json_str = cJSON_PrintUnformatted(json_data);
    if (json_str == NULL) {
        printf("Error: Failed to convert JSON data to string.\n");
        return;
    }

    // Check if the JSON data is valid
    cJSON* parsed_data = cJSON_Parse(json_str);
    free(json_str);
    if (parsed_data == NULL) {
        printf("Error: Invalid JSON data.\n");
        return;
    }

    // Check if the parsed data has a timestamp key
    cJSON* timestamp = cJSON_GetObjectItem(parsed_data, "timestamp");
    if (timestamp == NULL) {
        // If the timestamp key is not present, add it with the current UNIX time
        timestamp = cJSON_CreateNumber(time(NULL));
        cJSON_AddItemToObject(parsed_data, "timestamp", timestamp);
    }

    // Replace or add the key-value pair in the root JSON object
    cJSON* existing_data = cJSON_GetObjectItem(root, key);
    if (existing_data != NULL) {
        cJSON_ReplaceItemInObject(root, key, parsed_data);
    } else {
        cJSON_AddItemToObject(root, key, parsed_data);
    }

    // printf("Stored JSON data:\n%s\n", cJSON_Print(root));

    FILE* file = fopen(SERIAL_FILE_PATH, "wb");
    if (file == NULL) {
        printf("Error: Failed to open the serial file for writing.\n");
        return;
    }

    char* serialized_data = cJSON_PrintUnformatted(root);
    fwrite(serialized_data, strlen(serialized_data), 1, file);

    fclose(file);
    free(serialized_data);
}

void erase_json(cJSON* root, const char* key) {
    if (root == NULL) {
        printf("Error: Failed to load the serial file.\n");
        return;
    }

    cJSON_DeleteItemFromObject(root, key);

    FILE* file = fopen(SERIAL_FILE_PATH, "wb");
    if (file == NULL) {
        printf("Error: Failed to open the serial file for writing.\n");
        return;
    }

    char* serialized_data = cJSON_PrintUnformatted(root);
    fwrite(serialized_data, strlen(serialized_data), 1, file);

    fclose(file);
    free(serialized_data);
}

void get_serialized_keys() {
    cJSON* root = load_serial_file();
    if (root == NULL) {
        printf("Error: Failed to load the serial file.\n");
        return;
    }

    cJSON* child = root->child;
    while (child != NULL) {
        printf("%s\n", child->string);
        child = child->next;
    }

    cJSON_Delete(root);
}