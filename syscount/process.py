import re
import statistics
import matplotlib.pyplot as plt
import numpy as np

def get_stats_and_plot(log_file_path, label, color):
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

    # Return the means for graphing
    return req_sec_stats['mean'], transfer_sec_stats['mean']

# Labels and colors for each scenario
scenarios = ['Userspace', 'No Syscount', 'Kernel Filter', 'kernel-untarget', 'Userspace Filter']
colors = ['blue', 'green', 'red', 'orange', "yellow"]

# File paths
file_paths = [
    "result/userspace.txt",
    "result/no-syscount.txt",
    "result/kernel_targeted-filter.txt",
    "result/kernel_untargeted-filter.txt",
    "result/userspace-untargeted.txt"
]

# Gather data
req_sec_means = []
transfer_sec_means = []
for file_path, label in zip(file_paths, scenarios):
    req_sec_mean, transfer_sec_mean = get_stats_and_plot(file_path, label, colors[scenarios.index(label)])
    req_sec_means.append(req_sec_mean)
    transfer_sec_means.append(transfer_sec_mean)

x = np.arange(len(scenarios))  # the label locations

# Plotting the bar graph for Requests/sec
plt.figure(figsize=(10, 8))
plt.bar(x - 0.2, req_sec_means, 0.4, label='Requests/sec', color=colors)
plt.xlabel('Scenario', fontsize=16)
plt.ylabel('Requests per Second (RPS)', fontsize=16)
plt.xticks(x, scenarios, fontsize=12)
plt.legend()
plt.title('Average Requests per Second by Scenario')
plt.tight_layout()
plt.savefig('average_rps_by_scenario.png')
plt.show()

# Plotting the bar graph for Transfer/sec
plt.figure(figsize=(10, 8))
plt.bar(x + 0.2, transfer_sec_means, 0.4, label='Transfer/sec', color=colors)
plt.xlabel('Scenario', fontsize=16)
plt.ylabel('Transfer per Second (MB/sec)', fontsize=16)
plt.xticks(x, scenarios, fontsize=12)
plt.legend()
plt.title('Average Transfer per Second by Scenario')
plt.tight_layout()
plt.savefig('average_transfer_by_scenario.png')
plt.show()
