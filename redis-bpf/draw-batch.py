import matplotlib.pyplot as plt
import numpy as np

# Data from the new test results
batch_sizes = ['No AOF', '48', '24', '12', '3', '1 (Sync)', 'AOF On']
throughput_new = [87435.52, 52562.42, 50309.40, 48081.55, 43200.27, 22324.42, 12584.47]  # requests per second
average_latency_new = [0.046, 0.083, 0.087, 0.090, 0.104, 0.212, 0.379]  # milliseconds
max_latency_new = [1.623, 2.143, 5.495, 6.575, 3.159, 3.119, 8.375]  # milliseconds

# Colors for each batch size and method
colors_new = ['royalblue', 'cornflowerblue', 'lightsteelblue', 'lightblue', 'skyblue', 'orange', 'red']

# Creating the figure and axis objects again with the updated settings
fig, ax1 = plt.subplots(figsize=(10, 6))

# Plotting the throughput with different colors for each batch size
ax1.set_xlabel('Configuration')
ax1.set_ylabel('Throughput (req/sec)', color='black')
for i in range(len(batch_sizes)):
    ax1.bar(batch_sizes[i], throughput_new[i], color=colors_new[i], width=bar_width)
ax1.tick_params(axis='y', labelcolor='black')

# Creating a second y-axis for average latency with the same color scheme
ax2 = ax1.twinx()
ax2.set_ylabel('Average Latency (ms)', color='tab:blue')
ax2.plot(batch_sizes, average_latency_new, color='tab:blue', marker='o')
ax2.tick_params(axis='y', labelcolor='tab:blue')

# # Creating a third y-axis for max latency
# ax3 = ax1.twinx()
# ax3.spines['right'].set_position(('outward', 60))
# ax3.set_ylabel('Max Latency (ms)', color='tab:green')
# ax3.plot(batch_sizes, max_latency_new, color='tab:green', marker='^')
# ax3.tick_params(axis='y', labelcolor='tab:green')

# Setting the title and layout
plt.title('Redis Performance: IO_uring Hotpatching and AOF Impact')
fig.tight_layout()
plt.savefig('batch-redis.png')
# Show the updated plot
plt.show()
