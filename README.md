# Serialize JSON using Clang

Serialization is the process of converting data structures or objects into a format that can be stored or transmitted and later
reconstructed back into the original form. In the context of your code, serialization refers to converting JSON data into a string
representation that can be stored in a file.

The purpose of serialization is to facilitate the persistence or transfer of data. By serializing JSON data and storing it in a file, you
can save the data for later use or share it with other systems or applications. The serialized JSON data can be easily read from the file
and deserialized back into its original form when needed.

In your code, the serialize_json function takes a JSON data string, parses it into a JSON object using the cJSON library, and then
serializes the JSON object into a string representation using the cJSON_Print function. This serialized JSON string is then stored in a
file.

When you want to retrieve the stored JSON data, the load_serial_file function reads the serialized JSON string from the file, parses it back
into a JSON object using the cJSON library, and returns the JSON object. You can then access and manipulate the JSON data as needed.

So, the purpose of serialization in this context is to provide a way to persistently store JSON data in a file and later retrieve and use it
when required. It allows you to maintain the structure and content of the JSON data even when the program is not running or when the data
needs to be shared with other systems or applications.

## Compilation

The compilation commands for C code are generally the same across different platforms, with slight variations depending on the specific
toolchain and libraries available on each platform. Here are examples of compilation commands for different platforms:

Create static binary:

```bash
gcc -o serialize_data main.c utils.c cJSON.c -lm -static
```

macOS (arm64):

```bash
arch -arch arm64 gcc -o serialize_data main.c utils.c cJSON.c -lm
```

All other systems:

```bash
gcc -o serialize_data main.c utils.c cJSON.c -lm
```

## Usage

Please note that these are general examples, and depending on the specific setup of your system, you may need to install additional packages
or specify different compiler options. It's always a good idea to refer to the documentation or package manager of your specific platform
for more accurate instructions on how to compile C code.

Here's an example of how to use each function and a brief description of what it does:

serialize_json(const char\* json_data): This function takes a JSON string as input and prints the serialized version of the JSON data. It
uses the cJSON library to parse and serialize the JSON.

deserialize_json(const char\* serialized_data): This function takes a serialized JSON string as input and prints the deserialized version of
the JSON data. It uses the cJSON library to parse and deserialize the JSON.

`store_json(const char* key, const char* json_data)`: This function stores JSON data in the serial file. It takes a key and a JSON string as
input, adds the key-value pair to the existing JSON in the file, and updates the serial file with the modified JSON.

`erase_json(const char\* key)`: This function erases a key-value pair from the serial file. It takes a key as input, loads the existing JSON
from the file, removes the specified key-value pair from the JSON, and updates the serial file with the modified JSON.

`get_serialized_keys()`: This function retrieves and prints all the keys present in the serialized JSON data in the file.

`query_json(const char\* key)`: This function retrieves and prints the value associated with a specific key from the serialized JSON data in
the file. If the key does not exist, it prints an error message.

These functions can be used by passing appropriate arguments and running the program with the desired operation. For example:

To serialize JSON data:

```bash
./serialize_data serialize '{"name": "John", "age": 30}'
```

To deserialize serialized JSON data:

```bash
./serialize_data deserialize '{"name": "John", "age": 30}'
```

To store JSON data with a specific key:

```bash
./serialize_data store mykey '{"name": "John", "age": 30}'
```

To erase a key-value pair from the stored JSON data:

```bash
./serialize_data erase mykey
```

To retrieve all the keys from the stored JSON data:

```bash
./serialize_data keys
```

To query and retrieve the value associated with a specific key:

```bash
./serialize_data query mykey
```

## How it works

Let's go through the process and explain how it works:

Serialization: When you run the program with the "serialize" command, it expects a JSON data string as an argument. This JSON data is parsed
using cJSON library to create a JSON object (cJSON\* root). The serialize_json function takes this JSON object, converts it to a serialized
string using cJSON_Print, and stores it in a file named "serial.json". The serialized data represents the JSON structure as a string.

Deserialization: When you run the program with the "deserialize" command, it reads the serialized data from the "serial.json" file using the
load_serial_file function. The serialized data is then parsed using cJSON library to recreate the JSON object (cJSON\* root). The
deserialize_json function takes this JSON object and performs any desired operations on it.

Querying: When you run the program with the "query" command and provide a key as an argument, it loads the serialized data from the file
using load_serial_file. It then searches for the specified key within the JSON object (cJSON\* root) and prints the corresponding value if
the key exists. The query_json function handles this operation.

Storing: When you run the program with the "store" command, it expects a key and a JSON data string as arguments. It loads the serialized
data from the file using load_serial_file, parses the new JSON data string to create a JSON object (cJSON* new_data), and adds it to the
existing JSON object (cJSON* root) under the specified key. The modified JSON object is then serialized back to a string using cJSON_Print
and stored in the "serial.json" file.

Erasing: When you run the program with the "erase" command and provide a key as an argument, it loads the serialized data from the file
using load_serial_file. It searches for the specified key within the JSON object (cJSON\* root) and removes the corresponding key-value pair
if it exists. The modified JSON object is then serialized back to a string using cJSON_Print and stored in the "serial.json" file.

Listing Keys: When you run the program with the "keys" command, it loads the serialized data from the file using load_serial_file and prints
all the keys present in the JSON object (cJSON\* root).

The JSON file "serial.json" serves as a persistent storage for the JSON data. It allows you to save the JSON object to a file and retrieve
it later. The file is in a serialized format, which means the JSON object is converted to a string representation before storing it. This
allows you to easily store and retrieve complex JSON structures.

By using this approach, you can manipulate JSON data by serializing, deserializing, querying, storing, erasing, and listing keys. The
program provides a simple command-line interface for performing these operations on the JSON data stored in the "serial.json" file.
