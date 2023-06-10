#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h"
#include "utils.h"

#define SERIAL_FILE_PATH "serial.json"

cJSON* load_serial_file() {
    FILE* file = fopen(SERIAL_FILE_PATH, "r");
    if (file == NULL) {
        printf("Error: Failed to open the serial file.\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* json_data = (char*)malloc(file_size + 1);
    if (json_data == NULL) {
        printf("Error: Failed to allocate memory for serial file data.\n");
        fclose(file);
        return NULL;
    }

    fread(json_data, file_size, 1, file);
    json_data[file_size] = '\0';

    fclose(file);

    cJSON* root = cJSON_Parse(json_data);
    if (root == NULL) {
        printf("Error: Failed to parse the serial file JSON.\n");
        free(json_data);
        return NULL;
    }

    free(json_data);
    return root;
}

void serialize_json(const char* json_data) {
    cJSON* root = cJSON_Parse(json_data);
    if (root == NULL) {
        printf("Error: Failed to parse JSON data.\n");
        return;
    }

    char* serialized_data = cJSON_PrintUnformatted(root);
    if (serialized_data != NULL) {
        printf("%s\n", serialized_data);
        free(serialized_data);
    }

    cJSON_Delete(root);
}

void deserialize_json(const char* serialized_data) {
    cJSON* root = cJSON_Parse(serialized_data);
    if (root == NULL) {
        return;
    }

    cJSON* data = cJSON_GetObjectItem(root, "message");
    if (cJSON_IsString(data)) {
        printf("%s\n", data->valuestring);
    }

    cJSON_Delete(root);
}

void query_json(const char* key) {
    cJSON* root = load_serial_file();
    if (root == NULL) {
        return;
    }

    cJSON* json_data = cJSON_GetObjectItem(root, key);
    if (json_data == NULL) {
        cJSON_Delete(root);
        return;
    }

    printf("%s\n", cJSON_Print(json_data));

    cJSON_Delete(root);
}

void store_json(const char* key, const char* json_data) {
    // Load existing JSON data from the file
    cJSON* root = load_serial_file();
    if (root == NULL) {
        // Error loading the file, handle accordingly
        return;
    }

    cJSON* new_data = cJSON_Parse(json_data);
    if (new_data == NULL) {
        // Error parsing the new JSON data, handle accordingly
        cJSON_Delete(root);
        return;
    }

    // Check if the new data has a timestamp key
    cJSON* timestamp = cJSON_GetObjectItem(new_data, "timestamp");
    if (timestamp == NULL) {
        // If the timestamp key is not present, add it with the current UNIX time
        timestamp = cJSON_CreateNumber(time(NULL));
        cJSON_AddItemToObject(new_data, "timestamp", timestamp);
    }

    // Add the new key-value pair to the existing JSON
    cJSON_AddItemToObject(root, key, new_data);

    // Convert the updated JSON to a string
    char* serialized_data = cJSON_Print(root);
    printf("Stored JSON data:\n%s\n", serialized_data);

    // Store the updated JSON in the serial file
    FILE* file = fopen(SERIAL_FILE_PATH, "w");
    if (file == NULL) {
        printf("Error: Failed to open the serial file for writing.\n");
        cJSON_Delete(root);
        cJSON_Delete(new_data);
        free(serialized_data);
        return;
    }

    fputs(serialized_data, file);

    fclose(file);
    cJSON_Delete(root);
    cJSON_Delete(new_data);
    free(serialized_data);
}

void erase_json(const char* key) {
    cJSON* root = load_serial_file();
    if (root == NULL) {
        printf("Error: Failed to load the serial file.\n");
        return;
    }

    cJSON* json_data = cJSON_GetObjectItem(root, key);
    if (json_data == NULL) {
        printf("Error: Key '%s' not found in the serial file.\n", key);
        cJSON_Delete(root);
        return;
    }

    cJSON_DeleteItemFromObject(root, key);

    // Save the modified JSON back to the serial file
    FILE* file = fopen(SERIAL_FILE_PATH, "w");
    if (file == NULL) {
        printf("Error: Failed to open the serial file for writing.\n");
        cJSON_Delete(root);
        return;
    }

    char* serialized_data = cJSON_Print(root);
    if (serialized_data != NULL) {
        fputs(serialized_data, file);
        free(serialized_data);
    }

    fclose(file);
    cJSON_Delete(root);
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