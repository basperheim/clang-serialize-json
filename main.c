/*
  Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "utils.h"

#define AUTHOR "Benji Asperheim"
#define TITLE "Serialize JSON"
#define DESCRIPTION "A command-line interface (CLI) application to store and manipulate data by serializing it into a JSON file."
#define VERSION "0.0.2"

void printVersion() {
    printf("Author: %s\n", AUTHOR);
    printf("Title: %s\n", TITLE);
    printf("Description: %s\n", DESCRIPTION);
    printf("Version: %s\n", VERSION);
}

#define SERIAL_FILE_PATH "serial.json"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Error: No operation specified.\n");
        printf("Available commands: serialize, deserialize, query, store, erase, keys\n");
        return 1;
    }

    if (argc == 2 && strcmp(argv[1], "--version") == 0) {
        printVersion();
        return 0;
    }

    FILE* file = fopen(SERIAL_FILE_PATH, "r");
    if (file == NULL) {
        printf("Error: Failed to open the serial file.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* serial_data = malloc(file_size + 1);
    if (serial_data == NULL) {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return 1;
    }

    fread(serial_data, file_size, 1, file);
    serial_data[file_size] = '\0';

    fclose(file);

    cJSON* root = cJSON_Parse(serial_data);
    free(serial_data);

    if (root == NULL) {
        printf("Error: Failed to parse the serial file.\n");
        return 1;
    }

    const char* operation = argv[1];

    if (strcmp(operation, "serialize") == 0) {
        if (argc < 3) {
            printf("Error: No JSON data provided for serialization.\n");
            cJSON_Delete(root);
            return 1;
        }
        const char* json_data = argv[2];
        serialize_json(json_data);
    } else if (strcmp(operation, "deserialize") == 0) {
        deserialize_json(root);
    } else if (strcmp(operation, "query") == 0) {
        if (argc < 4) {
            printf("Error: Insufficient arguments for query operation.\n");
            cJSON_Delete(root);
            return 1;
        }
        const char* key = argv[2];
        const char* query = argv[3];
        query_json(root, key, query);
    } else if (strcmp(operation, "store") == 0) {
        if (argc < 4) {
            printf("Error: Insufficient arguments for store operation.\n");
            cJSON_Delete(root);
            return 1;
        }
        const char* key = argv[2];
        const char* json_data = argv[3];
        store_json(root, key, json_data);
    } else if (strcmp(operation, "erase") == 0) {
        if (argc < 3) {
            printf("Error: No key provided for erase operation.\n");
            cJSON_Delete(root);
            return 1;
        }
        const char* key = argv[2];
        erase_json(root, key);
    } else if (strcmp(operation, "keys") == 0) {
        get_serialized_keys();
    } else if (strcmp(operation, "fetch") == 0) {
        if (argc < 3) {
            printf("Error: No key provided for fetch operation.\n");
            cJSON_Delete(root);
            return 1;
        }
        const char* key = argv[2];
        fetch_json(root, key);
    } else {
        printf("Error: Unknown operation '%s'.\n", operation);
        return 1;
    }

    cJSON_Delete(root);
    return 0;
}