import matplotlib.pyplot as plt

# Data for the benchmark results
configurations = ['Without Module', 'bpftime Module', 'Lua Module', 'WASM Module']
requests_per_sec = [332686.27, 328324.01, 313901.63, 320276.19]
transfer_per_sec = [98.67, 97.38, 93.10, 94.99]  # In MB
latency_avg = [296.92, 301.57, 314.69, 309.60]  # In microseconds

# # Creating subplots
# fig, axs = plt.subplots(3, 1, figsize=(10, 12))

# # Plotting Requests per Second
# axs[0].bar(configurations, requests_per_sec, color='blue')
# axs[0].set_title('Requests per Second')
# axs[0].set_ylabel('Requests/sec')

# # Plotting Transfer per Second
# axs[1].bar(configurations, transfer_per_sec, color='green')
# axs[1].set_title('Data Transfer per Second')
# axs[1].set_ylabel('Transfer/sec (MB)')

# # Plotting Average Latency
# axs[2].bar(configurations, latency_avg, color='red')
# axs[2].set_title('Average Latency')
# axs[2].set_ylabel('Latency (µs)')

# # Adjust layout
# plt.tight_layout()

# # Show the plot
# plt.show()

# Adjusting the plot to include only Requests per Second and Latency, with narrower bars and different colors

# Reducing the bar width
bar_width = 0.4

# Setting different colors for each configuration
colors = ['skyblue', 'orange', 'lightgreen', 'purple']

# Creating subplots
fig, axs = plt.subplots(2, 1, figsize=(8, 10))

# Plotting Requests per Second
axs[0].bar(configurations, requests_per_sec, color=colors, width=bar_width)
axs[0].set_title('Requests per Second')
axs[0].set_ylabel('Requests/sec')

# Plotting Average Latency
axs[1].bar(configurations, latency_avg, color=colors, width=bar_width)
axs[1].set_title('Average Latency')
axs[1].set_ylabel('Latency (µs)')

# Adjust layout
plt.tight_layout()
plt.savefig('nginx-module.png')
# Show the plot
plt.show()
