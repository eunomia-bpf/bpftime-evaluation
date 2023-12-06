import matplotlib.pyplot as plt
import numpy as np

plt.rcParams.update(
    {
        "font.size": 16,
        "axes.labelsize": 18,
        "axes.titlesize": 20,
        "xtick.labelsize": 16,
        "ytick.labelsize": 16,
        "legend.fontsize": 16,
    }
)

import matplotlib.pyplot as plt
import numpy as np

bar_width = 0.4

# Data from the first test results
methods = ["io_uring sync(Batch 1)", "io_uring async", "Kernel & User async"]
throughput = [19031.67, 39677.82, 64724.92]  # requests per second
average_latency = [0.249, 0.114, 0.066]  # milliseconds

# Data from the new test results
batch_sizes = ["No AOF", "everysec", "Batch 48", "Batch 24", "Batch 12", "Batch 3", "alwayson"]
throughput_new = [
    87435.52,
    71669.17,
    52562.42,
    50309.40,
    48081.55,
    43200.27,
    12584.47,
]  # requests per second
average_latency_new = [
    0.046,
    0.057,
    0.083,
    0.087,
    0.090,
    0.104,
    0.379,
]  # milliseconds

# Merging the two test result data into one figure

# Data from the first and second test results
methods_combined = methods + batch_sizes
throughput_combined = throughput + throughput_new
average_latency_combined = average_latency + average_latency_new

# Colors for the combined data
colors_combined = ["skyblue", "orange", "lightgreen", "purple"] + [
    "royalblue",
    "cornflowerblue",
    "lightsteelblue",
    "lightblue",
    "skyblue",
    "orange",
    "red",
    "green",
]
# Sorting the data by throughput
sorted_indices = np.argsort(throughput_combined)
methods_sorted = np.array(methods_combined)[sorted_indices]
throughput_sorted = np.array(throughput_combined)[sorted_indices]
average_latency_sorted = np.array(average_latency_combined)[sorted_indices]
colors_sorted = np.array(colors_combined)[sorted_indices]
fig, ax1 = plt.subplots(figsize=(12, 8))

ax1.set_xlabel("Method / Configuration")
ax1.set_ylabel("Throughput (req/sec)", color="black")
bars = ax1.bar(methods_sorted, throughput_sorted, color=colors_sorted, width=bar_width)
ax1.tick_params(axis='y', labelcolor='black')
ax1.set_xticklabels(methods_sorted, rotation=45, ha="right")

# Add grid for better alignment
ax1.grid(True)

# Adjust the position of data labels for clarity
for bar in bars:
    yval = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2.0, yval + 0.05 * max(throughput_sorted), f"{yval:.2f}", va='center', ha='center')

fig.tight_layout()

plt.show()
plt.savefig("redis_enhanced.png")