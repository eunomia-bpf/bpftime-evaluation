import matplotlib.pyplot as plt
import numpy as np

# Data from the test results
methods = ['No Attach', 'IO_uring sync', 'IO_uring async', 'Kernel & User async']
throughput = [15303.39, 19031.67, 39677.82, 64724.92]  # requests per second
average_latency = [0.309, 0.249, 0.114, 0.066]  # milliseconds

# # Creating the figure and axis objects
# fig, ax1 = plt.subplots()

# # Plotting the throughput
# color = 'tab:red'
# ax1.set_xlabel('Method')
# ax1.set_ylabel('Throughput (req/sec)', color=color)
# ax1.bar(methods, throughput, color=color)
# ax1.tick_params(axis='y', labelcolor=color)

# # Creating a second y-axis for average latency
# ax2 = ax1.twinx()
# color = 'tab:blue'
# ax2.set_ylabel('Average Latency (ms)', color=color)
# ax2.plot(methods, average_latency, color=color, marker='o')
# ax2.tick_params(axis='y', labelcolor=color)

# # Setting the title and layout
# plt.title('Performance Comparison for Different Fsync Handling Approaches')
# fig.tight_layout()

# # # Show the plot
# # plt.show()

# Changing color for each method and adjusting bar width
colors_throughput = ['skyblue', 'orange', 'lightgreen', 'purple']
bar_width = 0.4  # Making the bars less wide

# Creating the figure and axis objects again with the updated settings
fig, ax1 = plt.subplots()

# Plotting the throughput with different colors for each method
ax1.set_xlabel('Method')
ax1.set_ylabel('Throughput (req/sec)', color='black')
for i in range(len(methods)):
    ax1.bar(methods[i], throughput[i], color=colors_throughput[i], width=bar_width)
ax1.tick_params(axis='y', labelcolor='black')

# Creating a second y-axis for average latency with the same color scheme
ax2 = ax1.twinx()
color = 'tab:blue'
ax2.set_ylabel('Average Latency (ms)', color=color)
ax2.plot(methods, average_latency, color=color, marker='o')
ax2.tick_params(axis='y', labelcolor=color)

# Setting the title and layout
plt.title('Performance Comparison for Async Fsync in redis AOF')
fig.tight_layout()

# save
plt.savefig('kernel-sync.png')
# Show the updated plot
plt.show()

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
