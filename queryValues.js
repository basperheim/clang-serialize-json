const { exec } = require("child_process");

// Call the C program and get the output
exec("./serialize_data query value 10", (error, stdout, stderr) => {
    if (error) {
        console.log(`error: ${error.message}`);
        return;
    }
    if (stderr) {
        console.log(`stderr: ${stderr}`);
        return;
    }

    // Parse the JSON output
    let data = JSON.parse(stdout);

    // Now you can access the data as a normal JavaScript object
    data.forEach((item) => {
        console.log(item.outer_key, item.data.value);
    });
});
