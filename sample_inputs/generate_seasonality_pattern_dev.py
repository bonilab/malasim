import pandas as pd

# Load the uploaded CSV file
file_path = "dev_seasonality.csv"

# Read the CSV file
df = pd.read_csv(file_path, header=None)

# Ensure there is exactly one row with 365 values
data_values = df.iloc[:, 0].tolist()

# print(data_values)

# Prepare the output format
output_text = "district_id," + ",".join([f"day{i+1}" for i in range(365)]) + "\n"
n_distrcits = 100
for i in range(n_distrcits):
    output_text += f"{i+1}," + ",".join(map(str, data_values)) + "\n"

# Save to a text file
output_file_path = "dev_seasonality_pattern.txt"
with open(output_file_path, "w") as f:
    f.write(output_text)
