import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl

mpl.rcParams['pdf.fonttype'] = 42
mpl.rcParams['ps.fonttype'] = 42

plt.rcParams.update({
    "font.size": 14,
    "axes.labelsize": 16,
    "axes.titlesize": 18,
    "xtick.labelsize": 14,
    "ytick.labelsize": 14,
    "legend.fontsize": 14,
})

# Data for the bar chart
labels = ["No AOF", "everysec", "alwayson", 
         "Batch 1", "Batch 3", "Batch 12", "Batch 24", "Batch 48", 
         "delayed-fsync", "fsync fast-notify"]

# Values (in thousands)
values = [87, 72, 13, 19, 43, 48, 50, 53, 40, 65]
# Full values for exact display
throughput = [value * 1000 for value in values]

# Create figure and axis
fig, ax = plt.subplots(figsize=(8, 7))

# Define colors for each category
default_color = '#8FD1C2'  # Teal/mint for default configs
batching_color = '#F8C07F'  # Orange for batching configs
delayed_color = '#EF9A9A'  # Light red for delayed-fsync

# Create color list based on categories
colors = [default_color] * 3 + [batching_color] * 5 + [delayed_color] * 2

# Create the bar chart
bars = ax.bar(labels, throughput, color=colors)

# Configure the chart
ax.set_ylabel('Throughput (req/sec)')
ax.set_ylim(0, 90000)
ax.grid(True, linestyle='-', alpha=0.5, color='#CCCCCC')

# Rotate x-axis labels
plt.xticks(rotation=45, ha='right')

# Add value labels on top of bars
for bar in bars:
    height = bar.get_height()
    ax.annotate(f'{int(height/1000)}K',
                xy=(bar.get_x() + bar.get_width()/2, height),
                xytext=(0, 3),  # 3 points vertical offset
                textcoords="offset points",
                ha='center', va='bottom')

# Add legend
from matplotlib.patches import Patch
legend_elements = [
    Patch(facecolor=default_color, label='Default Configs'),
    Patch(facecolor=batching_color, label='Batching Configs'),
    Patch(facecolor=delayed_color, label='Delayed-fsync')
]
ax.legend(handles=legend_elements, loc='upper right')

# Adjust layout and save
plt.tight_layout()
plt.savefig("redis_performance.pdf")
plt.savefig("redis_performance.png", dpi=300)
plt.show()