import subprocess
import json, asyncio

async def serialize_data(data, key):
    command = f"./serialize_data store {key} '{json.dumps(data)}'"
    try:
        result = subprocess.run(command, shell=True, check=True)
        print(f"Data serialized with key: {key}")
    except subprocess.CalledProcessError as error:
        print("Serialization failed:", error)

async def generate_and_serialize_data():
    concurrency = 10  # Number of concurrent serialization tasks
    iterations = 50

    tasks = []
    for i in range(iterations):
        data = {"value": i}
        for j in range(concurrency):
            key = f"example_key_{i}_{j}"
            tasks.append(serialize_data(data, key))
        if i % 10 == 0:
            # Log after every 10th iteration
            print(f"Iteration {i}: Generating and serializing data")

    # Wait for all tasks to complete
    await asyncio.gather(*tasks)

asyncio.run(generate_and_serialize_data())