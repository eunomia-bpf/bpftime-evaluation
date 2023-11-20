import matplotlib.pyplot as plt
import numpy as np


# Helper function to plot the performance drop
def plot_performance_drop(performance_drop, title):
    sizes = sorted(list(performance_drop[next(iter(performance_drop))].keys()))
    sub_keys = list(performance_drop.keys())
    request_drops = {
        sub_key: [performance_drop[sub_key][size]["request_drop"] for size in sizes]
        for sub_key in sub_keys
    }
    transfer_drops = {
        sub_key: [performance_drop[sub_key][size]["transfer_drop"] for size in sizes]
        for sub_key in sub_keys
    }

    # Plotting
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 6))
    fig.suptitle(title, fontsize=16)

    # Plot for Request Drops
    for sub_key in sub_keys:
        ax1.plot(sizes, request_drops[sub_key], label=sub_key)
    ax1.set_xlabel("Size", fontsize=14)
    ax1.set_ylabel("Request Drop (%)", fontsize=14)
    ax1.legend(fontsize=12)
    ax1.tick_params(axis="both", which="major", labelsize=12)
    ax1.set_title("Request Performance Drop", fontsize=14)

    # Plot for Transfer Drops
    for sub_key in sub_keys:
        ax2.plot(sizes, transfer_drops[sub_key], label=sub_key)
    ax2.set_xlabel("Size", fontsize=14)
    ax2.set_ylabel("Transfer Drop (%)", fontsize=14)
    ax2.legend(fontsize=12)
    ax2.tick_params(axis="both", which="major", labelsize=12)
    ax2.set_title("Transfer Performance Drop", fontsize=14)

    plt.tight_layout(rect=[0, 0.03, 1, 0.95])
    return plt


# Plotting for HTTPS
plt_https = plot_performance_drop(https_performance_drop, "HTTPS Performance Drop")
plt_https.show()

# Plotting for HTTP
plt_http = plot_performance_drop(http_performance_drop, "HTTP Performance Drop")
plt_http.show()
