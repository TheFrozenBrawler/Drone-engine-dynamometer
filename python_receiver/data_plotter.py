import matplotlib.pyplot as plt
import os
import pandas as pd
import mplcursors

csv_files = []
i = 1
file_selection = True

while(file_selection):
    print("Chose csv file to read from:")
    for file in os.listdir():
        if file[-4:] == ".csv":
            csv_files.append(file)
            print(f"{i}.", file)
            i += 1

    print("Write number of chosen file:")
    file_nb = int(input())
    file_nb -= 1

    if file_nb >= 0 and file_nb <= len(csv_files) - 1:
        file_selection = False        
    else:
        print("Wrong file number, chose one from the list.")

"""Load CSV data using pandas for better performance."""
try:
    df = pd.read_csv(csv_files[file_nb], delimiter=",")
except Exception as e:
    print(f"Error loading file: {e}")
    exit()

# PLOTTING - Create 4 subplots similar to python_receiver.py
fig, axes = plt.subplots(4, 1, figsize=(8.5, 6.5))
fig.canvas.manager.set_window_title('Data Plotter - Visualization')

# Calculate relative time in seconds from the first timestamp
if 'Timestamp' in df.columns:
    df['Timestamp'] = pd.to_datetime(df['Timestamp'])
    start_time = df['Timestamp'].iloc[0]
    x = (df['Timestamp'] - start_time).dt.total_seconds()  # Convert to seconds
else:
    x = df.index

# Subplot 1: Temperatures
ax1 = axes[0]
if 'Temperature_AHT20' in df.columns:
    ax1.plot(x, df['Temperature_AHT20'], label='Air T sensor [°C]', linewidth=2)
if 'Temperature_MLXA' in df.columns:
    ax1.plot(x, df['Temperature_MLXA'], label='Laser T sensor A [°C]', linewidth=2)
if 'Temperature_MLXB' in df.columns:
    ax1.plot(x, df['Temperature_MLXB'], label='Laser T sensor B [°C]', linewidth=2)
ax1.set_ylabel('Temperature [°C]')
ax1.set_title('Temperature Sensors')
ax1.grid(True)
ax1.legend(loc='lower left')

# Subplot 2: Power Supply Sensors
ax2 = axes[1]
ax2_twin = ax2.twinx()
if 'Power_supply_current' in df.columns:
    ax2.plot(x, df['Power_supply_current'] / 1000, label='Current [A]', color='blue', linewidth=2)
if 'Power_supply_voltage' in df.columns:
    ax2_twin.plot(x, df['Power_supply_voltage'], label='Voltage [V]', color='red', linewidth=2)
ax2.set_ylabel('Current [A]', color='blue')
ax2_twin.set_ylabel('Voltage [V]', color='red')
ax2.set_title('Power Supply Sensors')
ax2.grid(True)

# Combine legends from both axes
lines1, labels1 = ax2.get_legend_handles_labels()
lines2, labels2 = ax2_twin.get_legend_handles_labels()
ax2.legend(lines1 + lines2, labels1 + labels2, loc='lower left')

# Subplot 3: Airspeed
ax3 = axes[2]
if 'Air_speed' in df.columns:
    ax3.plot(x, df['Air_speed'], label='Airspeed [m/s]', color='purple', linewidth=2)
ax3.set_ylabel('Airspeed [m/s]')
ax3.set_title('Airspeed Sensor')
ax3.grid(True)
ax3.legend(loc='lower left')

# Subplot 4: Tensometer
ax4 = axes[3]
if 'Tensometer' in df.columns:
    ax4.plot(x, df['Tensometer'] / 1000, label='Tensometer [kg]', color='brown', linewidth=2)
ax4.set_xlabel('Time [s] since start')
ax4.set_ylabel('Tensometer [kg]')
ax4.set_title('Tensometer Sensor')
ax4.grid(True)
ax4.legend(loc='lower left')

plt.tight_layout()

# Enable hover/crosshair tool to inspect values
mplcursors.cursor(hover=True)

plt.show()

