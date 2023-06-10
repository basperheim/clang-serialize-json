# Serialize JSON using Clang

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

To serialize JSON data: `./serialize_data serialize '{"name": "John", "age": 30}'` To deserialize serialized JSON data:

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
