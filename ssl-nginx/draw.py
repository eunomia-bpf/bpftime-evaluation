import matplotlib.pyplot as plt
import numpy as np
import matplotlib

# Increase font sizes globally
plt.rcParams.update({
    'font.size': 16,
    'axes.labelsize': 18,
    'axes.titlesize': 20,
    'xtick.labelsize': 16,
    'ytick.labelsize': 16,
    'legend.fontsize': 16,
})

# Data from the tables
data_sizes = ['1K', '2K', '4K', '16K', '128K', '256K']
requests_per_sec_kernel = [8344.41, 9180.44, 7620.01, 4688.97, 525.22, 242.59]
requests_per_sec_userbpf = [11011.06, 11028.57, 9904.57, 6473.85, 1167.10, 457.66]
requests_per_sec_native = [11702.14, 11828.92, 10883.19, 6763.48, 1369.12, 457.73]

# Convert string data sizes to numerical values for plotting
x = np.arange(len(data_sizes))
width = 0.25  # the width of the bars

# Create the plot with grid
plt.figure(figsize=(8, 6))
plt.grid(True, linestyle='-', alpha=0.7)

# Plotting the data with the colors from the image
plt.bar(x - width, requests_per_sec_kernel, width, label='Kernel Uprobe', color='#F9C27F')
plt.bar(x, requests_per_sec_userbpf, width, label='bpftime', color='#F98E7F')
plt.bar(x + width, requests_per_sec_native, width, label='Native', color='#7FC8C2')

# Adding labels and title
plt.xlabel('Data Size')
plt.ylabel('Requests/sec')
# Removed title since it's in the figure caption
plt.xticks(x, data_sizes)
plt.ylim(0, 12000)  # Set y-axis limit to match the image

# Position legend in the upper right as shown in the image
plt.legend(loc='upper right', framealpha=0.9)

# Add more padding around the plot for better visibility of labels
plt.tight_layout(pad=2.0)

plt.savefig('ssl-nginx.pdf', bbox_inches='tight')
# Show the plot
plt.show()
