import matplotlib.pyplot as plt
import numpy as np

# Data for Requests per Second (RPS) in different scenarios
scenarios = ['No Syscount', 'Kernel Syscount Globally', 'Kernel Target Filter on Nginx', 'Userspace Syscount']
rps_values = [12292.24,  10280.56, 11263.99, 8593.33]

# Convert scenario names to numerical values for plotting
x = np.arange(len(scenarios))

# Create the plot
plt.figure(figsize=(10, 6))

# Plotting the data
plt.bar(x, rps_values, 0.4, color=['orange', 'skyblue', 'green', 'red'])

# Adding labels and title
plt.xlabel('Syscount Scenario')
plt.ylabel('Requests per Second (RPS)')
plt.title('Impact of Syscount on Nginx Performance')
plt.xticks(x, scenarios)
plt.savefig('syscount.png')
# Show the plot
plt.show()

