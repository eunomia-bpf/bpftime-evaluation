import re
import statistics
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


# Updated data to reflect 'Native' as a separate category
categories = ['Native', 'Kernel', 'Userspace']
native_data = [12292]  # Native data
untargeted_data = [10413, 12302]  # Untargeted data for Kernel and Userspace
targeted_data = [9668, 8593]  # Targeted data for Kernel and Userspace

# The x locations for the groups
ind = np.arange(len(categories))

# Bar width
width = 0.35

# Create the plot
fig, ax = plt.subplots(figsize=(12, 10))


# Plotting the 'Native' category bar
native_bar = ax.bar(ind[0], native_data, width, label='Native', color='#8ecfc9')

# Plotting the 'Untargeted' and 'Targeted' category bars for Kernel and Userspace
untargeted_bar = ax.bar(ind[1:] - width/2, untargeted_data, width, label='Untargeted', color='#ffbe7a')
targeted_bar = ax.bar(ind[1:] + width/2, targeted_data, width, label='Targeted', color='#fa7f6f')

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('Requests per Second (Rps)')
# ax.set_title('syscount test with nginx')
ax.set_xticks(ind)
ax.set_xticklabels(categories)
ax.legend(loc='upper center', bbox_to_anchor=(0.48, 1.15), ncol=3)

# Function to attach a label above each bar in rects, displaying its height.
def autolabel(rects):
    for rect in rects:
        height = rect.get_height()
        ax.annotate('{}'.format(height),
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')

# Call the autolabel function for each set of bars
autolabel(native_bar)
autolabel(untargeted_bar)
autolabel(targeted_bar)

plt.savefig('syscount.pdf')
# Show the plotplt.savefig('average_transfer_by_scenario.png')
plt.show()
