import matplotlib.pyplot as plt
import pandas as pd

# Load the simulation output data
output_file = "/Users/refobic/Documents/DD1/Project1_G5/DD1_Project1_G5/DD1_Project1_G5/output.sim"
data = pd.read_csv(output_file, header=None, names=["time", "signal", "value"])

signals = data["signal"].unique() #Store the name of the signal
signals_per_page = 5  # To show how many signals are shown each page
total_pages = (len(signals) + signals_per_page - 1) // signals_per_page  # Calculate total pages

# Create each page of plots
for page in range(total_pages): #to loop in each page
    start_idx = page * signals_per_page 
    end_idx = min((page + 1) * signals_per_page, len(signals)) 
    page_signals = signals[start_idx:end_idx] #select subset of the signals to print

    fig, ax = plt.subplots(len(page_signals), 1, figsize=(10, 2 * len(page_signals)), sharex=True)

    if len(page_signals) == 1:
        ax = [ax]

    max_time = data["time"].max()

    # Create waveforms for each signal in the current page
    for i, signal in enumerate(page_signals):
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

    # Save each page as a separate file or display it
    plt.savefig(f"/Users/refobic/Documents/DD1/Project1_G5/DD1_Project1_G5/DD1_Project1_G5/waveform_page_{page + 1}.png")
    plt.close()
    print(f"Waveform page {page + 1} saved as waveform_page_{page + 1}.png")
