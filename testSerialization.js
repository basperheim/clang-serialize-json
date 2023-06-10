const { exec } = require("child_process");

const serializeData = async (data, key) => {
    const command = `./serialize_data store ${key} '${JSON.stringify(data)}'`;
    try {
        await exec(command);
        console.log(`Data serialized with key: ${key}`);
    } catch (error) {
        console.error("Serialization failed:", error);
    }
};

const generateAndSerializeData = async () => {
    const concurrency = 10; // Number of concurrent serialization tasks
    const iterations = 50;

    const promises = [];
    for (let i = 0; i < iterations; i++) {
        const data = { value: i };
        for (let j = 0; j < concurrency; j++) {
            const key = `example_key_${i}_${j}`;
            promises.push(serializeData(data, key));
        }
        if (i % 10 === 0) {
            // Log after every 10th iteration
            console.log(`Iteration ${i}: Generating and serializing data`);
        }
    }

    await Promise.all(promises);
};

generateAndSerializeData();
