import re
import statistics

# Define the path to the log file
log_file_path = 'bpftime-evaluation/syscount/test-log.txt'

# Regular expressions to extract the numerical values for requests per second and transfer per second
req_sec_pattern = re.compile(r"Requests/sec:\s+(\d+\.\d+)")
transfer_sec_pattern = re.compile(r"Transfer/sec:\s+(\d+\.\d+)MB")

# Lists to store the values extracted from the log file
req_sec_values = []
transfer_sec_values = []

# Open the log file and extract the values
with open(log_file_path, 'r') as file:
    log_contents = file.read()
    req_sec_values = [float(match) for match in req_sec_pattern.findall(log_contents)]
    transfer_sec_values = [float(match) for match in transfer_sec_pattern.findall(log_contents)]

# Function to calculate various statistics
def calculate_statistics(values):
    return {
        'mean': statistics.mean(values),
        'median': statistics.median(values),
        'stdev': statistics.stdev(values) if len(values) > 1 else 0,
        'min': min(values),
        'max': max(values)
    }

# Calculate statistics for Requests/sec and Transfer/sec
req_sec_stats = calculate_statistics(req_sec_values)
transfer_sec_stats = calculate_statistics(transfer_sec_values)

print("len: ", len(req_sec_values))

# Print the statistics
print("Requests/sec statistics:")
for stat, value in req_sec_stats.items():
    print(f"{stat.capitalize()}: {value:.2f}")

print("\nTransfer/sec statistics:")
for stat, value in transfer_sec_stats.items():
    print(f"{stat.capitalize()}: {value:.2f}")
