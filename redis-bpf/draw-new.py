import matplotlib.pyplot as plt
import numpy as np

plt.rcParams.update(
    {
        "font.size": 22,
        "axes.labelsize": 22,
        "axes.titlesize": 22,
        "xtick.labelsize": 22,
        "ytick.labelsize": 22,
        "legend.fontsize": 22,
    }
)

# Creating a clustered bar chart for the provided data

# Data for the clustered bar chart
default_configs = ["No AOF", "everysec", "alwayson"]
default_throughput = [87435.52, 71669.17, 12584.47]

batching_configs = ["Batch 1", "Batch 3", "Batch 12", "Batch 24", "Batch 48"]
batching_throughput = [19031.67, 43200.27, 48081.55, 50309.40, 52562.42]

fsync_configs = ["delayed-fsync", "delayed-fsync fast-notify"]
fsync_throughput = [39677.82, 64724.92]

# Plotting
fig, ax = plt.subplots(figsize=(12, 10))

# Cluster positions
default_positions = np.arange(len(default_configs))
batching_positions = np.arange(len(batching_configs)) + len(default_configs) + 1 # Offset by 1 for spacing
fsync_positions = np.arange(len(fsync_configs)) + len(default_configs) + len(batching_configs) + 2 # Further offset

# Bar width
bar_width = 0.8

# Plotting the clusters
ax.bar(default_positions, default_throughput, width=bar_width, label="Default Configs", color='#8ecfc9')
ax.bar(batching_positions, batching_throughput, width=bar_width, label="Batching Configs", color='#ffbe7a')
ax.bar(fsync_positions, fsync_throughput, width=bar_width, label="Delayed-fsync", color='#fa7f6f')

# Labels and ticks
ax.set_ylabel('Throughput (req/sec)')
# ax.set_title('Redis Configuration Throughput Comparison')
ax.set_xticks(np.concatenate([default_positions, batching_positions, fsync_positions]))
ax.set_xticklabels(default_configs + batching_configs + fsync_configs, rotation=45, ha="right")

# Adding data labels
for pos, val in zip(np.concatenate([default_positions, batching_positions, fsync_positions]), 
                    np.concatenate([default_throughput, batching_throughput, fsync_throughput])):
    ax.text(pos, val, f'{val/1000:.0f}K', ha='center', va='bottom')

# Adding grid
ax.grid(True)

plt.legend()
plt.tight_layout()
plt.show()
plt.savefig("redis_cluster.pdf")
