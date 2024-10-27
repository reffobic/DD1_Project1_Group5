import matplotlib.pyplot as plt
import pandas as pd

# Load the simulation output data
output_file = "/Users/refobic/Documents/DD1/Project1_G5/DD1_Project1_G5/DD1_Project1_G5/output.sim"
data = pd.read_csv(output_file, header=None, names=["time", "signal", "value"])

# Extract unique signals
signals = data["signal"].unique()
fig, ax = plt.subplots(len(signals), 1, figsize=(10, 2 * len(signals)), sharex=True)

# Ensure `ax` is always a list of Axes objects for consistency
if len(signals) == 1:
    ax = [ax]

# Get the maximum time for extending the signal
max_time = data["time"].max()

# Create waveforms for each signal
for i, signal in enumerate(signals):
    signal_data = data[data["signal"] == signal].sort_values("time")
    time_values = [0] + list(signal_data["time"]) + [max_time]
    level_values = [signal_data["value"].iloc[0]] + list(signal_data["value"]) + [signal_data["value"].iloc[-1]]
    
    ax[i].step(time_values, level_values, where="post")
    ax[i].set_ylim(-0.5, 1.5)
    ax[i].set_yticks([0, 1])
    ax[i].set_yticklabels(['0', '1'])
    ax[i].set_ylabel(signal)
    ax[i].grid(True, which='both', axis='x', linestyle='--', linewidth=0.5)
    ax[i].set_title(f"Signal: {signal}")

# Set x-axis properties
ax[-1].set_xlabel("Time (units)")
plt.tight_layout()
plt.show()
