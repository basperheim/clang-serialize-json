import subprocess
import json

# Call the C program and get the output
output = subprocess.check_output(['./serialize_data', 'query', 'value', '10'])

# Parse the JSON output
data = json.loads(output)

# Now you can access the data as a normal Python dictionary
for item in data:
    print(item['outer_key'], item['data']['value'])