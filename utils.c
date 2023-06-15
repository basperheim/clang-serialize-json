#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "cJSON.h"
#include "utils.h"
#include <regex.h>

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

void serialize_json(const char* json_data) {
    if (json_data == NULL) {
        printf("Error: Invalid JSON data.\n");
        return;
    }

    FILE* file = fopen(SERIAL_FILE_PATH, "wb");
    if (file == NULL) {
        printf("Error: Failed to open the serial file for writing.\n");
        return;
    }

    fwrite(json_data, strlen(json_data), 1, file);

    fclose(file);
}

void query_json(const cJSON* root, const char* key, const char* query) {
    if (root == NULL) {
        printf("Error: Failed to load the serial file.\n");
        return;
    }

    if (key == NULL || strlen(key) == 0) {
        printf("Error: No key provided for query operation.\n");
        return;
    }

    regex_t regex;
    int reti;
    char msgbuf[100];

    /* Compile regular expression */
    reti = regcomp(&regex, query, REG_ICASE);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return;
    }

    cJSON *output_array = cJSON_CreateArray();

    cJSON *item = NULL;
    cJSON_ArrayForEach(item, root) {
        cJSON *inner = cJSON_GetObjectItemCaseSensitive(item, key);

        if (inner != NULL) {
            if (cJSON_IsString(inner)) {
                /* Execute regular expression */
                reti = regexec(&regex, inner->valuestring, 0, NULL, 0);
                if (!reti) {
                    cJSON *output_object = cJSON_CreateObject();
                    cJSON_AddItemToObject(output_object, "outer_key", cJSON_CreateString(item->string));
                    cJSON_AddItemToObject(output_object, "data", cJSON_Duplicate(item, 1));
                    cJSON_AddItemToArray(output_array, output_object);
                }
            } else if (cJSON_IsNumber(inner)) {
                /* Convert query to an integer */
                int query_int = atoi(query);
                if (inner->valueint == query_int) {
                    /* They match, so print the value */
                    cJSON *output_object = cJSON_CreateObject();
                    cJSON_AddItemToObject(output_object, "outer_key", cJSON_CreateString(item->string));
                    cJSON_AddItemToObject(output_object, "data", cJSON_Duplicate(item, 1));
                    cJSON_AddItemToArray(output_array, output_object);
                }
            }
        }
    }

    char *output_string = cJSON_Print(output_array);
    printf("%s\n", output_string);
    free(output_string);
    cJSON_Delete(output_array);

    /* Free compiled regular expression if you want to use the regex_t again */
    regfree(&regex);
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

void get_key(const cJSON* root, const char* key, const char* query, cJSON** result) {
    if (root == NULL) {
        *result = NULL;
        return;
    }

    cJSON* json_key = cJSON_GetObjectItem(root, key);
    if (json_key == NULL) {
        *result = NULL;
        return;
    }

    if (cJSON_IsString(json_key) && strcmp(json_key->valuestring, query) == 0) {
        *result = cJSON_Duplicate(json_key, true);
        return;
    }

    cJSON* child = root->child;
    while (child != NULL) {
        get_key(child, key, query, result);
        if (*result != NULL) {
            return;
        }
        child = child->next;
    }
}

void store_json(cJSON* root, const char* key, const char* json_data) {
    if (root == NULL) {
        printf("Error: Failed to load the serial file.\n");
        return;
    }

    if (json_data == NULL) {
        printf("Error: Invalid JSON data.\n");
        return;
    }

    cJSON* new_data = cJSON_Parse(json_data);
    if (new_data == NULL) {
        printf("Error: Failed to parse the new JSON data.\n");
        return;
    }

    // Check if the parsed data has a timestamp key
    cJSON* timestamp = cJSON_GetObjectItem(new_data, "timestamp");
    if (timestamp == NULL) {
        // If the timestamp key is not present, add it with the current UNIX time
        timestamp = cJSON_CreateNumber(time(NULL));
        cJSON_AddItemToObject(new_data, "timestamp", timestamp);
    }

    FILE* file = fopen(SERIAL_FILE_PATH, "r+");
    if (file == NULL) {
        printf("Error: Failed to open the serial file for writing.\n");
        cJSON_Delete(new_data);
        return;
    }

    // Read the existing file contents
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* file_contents = (char*)malloc(file_size + 1);
    if (file_contents == NULL) {
        printf("Error: Failed to allocate memory for existing file data.\n");
        fclose(file);
        cJSON_Delete(new_data);
        return;
    }

    fread(file_contents, file_size, 1, file);
    file_contents[file_size] = '\0';

    // Parse the existing data into a cJSON object
    cJSON* existing_root = cJSON_Parse(file_contents);
    free(file_contents);

    if (existing_root == NULL) {
        printf("Error: Failed to parse the existing file data.\n");
        fclose(file);
        cJSON_Delete(new_data);
        return;
    }

    // Replace the specific key-value pair in the existing cJSON object with the new data
    cJSON_DeleteItemFromObject(existing_root, key);  // Delete the item first
    cJSON_AddItemToObject(existing_root, key, new_data);  // Then add the new item
    new_data = NULL;  // Prevent double freeing

    // Write the updated cJSON object to a temporary file
    FILE* temp_file = fopen("temp.json", "w");
    if (temp_file == NULL) {
        printf("Error: Failed to create a temporary file.\n");
        fclose(file);
        cJSON_Delete(new_data);
        cJSON_Delete(existing_root);
        return;
    }

    char* serialized_data = cJSON_PrintUnformatted(existing_root);
    fwrite(serialized_data, strlen(serialized_data), 1, temp_file);

    fclose(temp_file);
    fclose(file);

    // Rename the temporary file to the original file
    if (rename("temp.json", SERIAL_FILE_PATH) != 0) {
        printf("Error: Failed to rename the temporary file.\n");
        remove("temp.json");  // Clean up the temporary file
    }

    cJSON_Delete(existing_root);
    free(serialized_data);
    cJSON_Delete(new_data);
}

void fetch_json(cJSON* root, const char* key) {
    if (root == NULL || key == NULL) {
        printf("Error: Invalid JSON root or key.\n");
        return;
    }

    cJSON* item = cJSON_GetObjectItem(root, key);
    if (item == NULL) {
        return;  // If the key does not exist, do nothing
    }

    // Print the key and the associated data
    char* string = cJSON_Print(item);
    printf("{\n\"%s\":\t%s\n}\n", key, string);
    free(string);
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

void list_keys(const cJSON* root, const char* key) {
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