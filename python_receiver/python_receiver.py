import serial
import csv
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import threading
import time
from datetime import datetime
from bisect import bisect_left

# --- Configuration ---
PORT = 'COM12'               # Arduino serial port
BAUDRATE = 115200             # baud rate
CSV_FILE = 'dane.csv'       # output CSV filename
# Time between subsequent UART reads (in seconds). Change this value
# to control the sampling rate.
SAMPLE_INTERVAL = 0.1       # seconds
# Plot refresh interval (in ms)
PLOT_INTERVAL_MS = 250      # milliseconds
PLOT_TIMESPAN = 3 * 60         # seconds - how much historical data to display on the plot

# --- Data buffers ---
timestamps = []  # time (seconds since start) for each sample
airspeed_values = []
temperature_aht20_values = []
temperature_mlxa_values = []
temperature_mlxb_values = []
tensometer_values = []
power_current_values = []
power_voltage_values = []
power_charge_values = []
power_temperature_values = []
running = True  # flag to stop the program
data_lock = threading.Lock()

# --- Function to read data from UART ---
def read_serial():
    global running
    try:
        ser = serial.Serial(PORT, BAUDRATE, timeout=1)
        print(f"[INFO] Connected with {PORT}")
    except serial.SerialException as e:
        print(f"[ERROR] Could not open port {PORT}: {e}")
        print("[ERROR] Make sure the port is not already in use and you have permission to access it.")
        running = False
        return

    # start time for relative timestamps
    start_time = time.time()

    print("[INFO] TENSOMETER CALIBRATION: remove all weight from the tensometer and press enter")
    input()
    ser.write(b"\n")

    with open(CSV_FILE, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        # write a human-readable timestamp, ADC value and voltage
        writer.writerow(['Timestamp', 'Air_speed', 'Temperature_AHT20', 'Temperature_MLXA', 'Temperature_MLXB', 'Tensometer', 'Power_supply_current', 'Power_supply_voltage', 'Power_supply_charge', 'Power_supply_temperature'])

        while running:
            line = ser.readline().decode('utf-8').strip()
            if ';' in line:
                try:
                    values = line.split(';')
                    # Expected: Air_speed, Temp_AHT20, Temp_MLXA, Temp_MLXB, Tensometer,
                    #           Current, Voltage, Charge, Temp_Power
                    if len(values) >= 9:
                        airspeed = float(values[0])
                        temp_aht20 = float(values[1])
                        temp_mlxa = float(values[2])
                        temp_mlxb = float(values[3])
                        tensometer = float(values[4])
                        power_current = float(values[5])
                        power_voltage = float(values[6])
                        power_charge = float(values[7])
                        power_temp = float(values[8])
                    else:
                        raise ValueError("Not enough values in line")
                except ValueError:
                    print("[INFO] Incorrect line received")
                    airspeed = temp_aht20 = temp_mlxa = temp_mlxb = tensometer = 0
                    power_current = power_voltage = power_charge = power_temp = 0

                # record timestamp: human-readable (ISO) and relative time in seconds
                iso_ts = datetime.now().isoformat(timespec='milliseconds')
                rel_ts = time.time() - start_time

                # append under lock to avoid concurrent access during plotting
                with data_lock:
                    timestamps.append(rel_ts)
                    airspeed_values.append(airspeed)
                    temperature_aht20_values.append(temp_aht20)
                    temperature_mlxa_values.append(temp_mlxa)
                    temperature_mlxb_values.append(temp_mlxb)
                    tensometer_values.append(tensometer)
                    power_current_values.append(power_current)
                    power_voltage_values.append(power_voltage)
                    power_charge_values.append(power_charge)
                    power_temperature_values.append(power_temp)

                # write to CSV (keep this outside the lock to minimize contention)
                writer.writerow([iso_ts, airspeed, temp_aht20, temp_mlxa, temp_mlxb,
                                tensometer, power_current, power_voltage, power_charge, power_temp])

    ser.close()
    print("[INFO] Connection with the port has been closed.")

# --- Function to update the plot ---
def update(frame):
    with data_lock:
        ts = timestamps.copy()
        ahs = airspeed_values.copy()
        ta = temperature_aht20_values.copy()
        tm_a = temperature_mlxa_values.copy()
        tm_b = temperature_mlxb_values.copy()
        pc = power_current_values.copy()
        pv = power_voltage_values.copy()
        pch = power_charge_values.copy()
        ptmp = power_temperature_values.copy()
        tens = tensometer_values.copy()

    if len(ts) > 0:
        # limit to last PLOT_TIMESPAN seconds
        end_t = ts[-1]
        start_threshold = end_t - PLOT_TIMESPAN
        start_idx = bisect_left(ts, start_threshold)

        x = ts[start_idx:]

        # Create 4 subplots
        plt.clf()
        fig = plt.gcf()

        # Subplot 1: Temperatures
        ax1 = fig.add_subplot(4, 1, 1)
        ax1.plot(x, ta[start_idx:], label='Air T sensor [°C]', linewidth=2)
        ax1.plot(x, tm_a[start_idx:], label='Laser T sensor A [°C]', linewidth=2)
        ax1.plot(x, tm_b[start_idx:], label='Laser T sensor B [°C]', linewidth=2)
        ax1.set_ylabel('Temperature [°C]')
        ax1.set_title('Temperature Sensors')
        ax1.grid(True)
        ax1.legend(loc='upper right')

        # Subplot 2: Power Supply Sensors
        ax2 = fig.add_subplot(4, 1, 2)
        ax2_twin = ax2.twinx()
        ax2.plot(x, pc[start_idx:], label='Current [A]', color='blue', linewidth=2)
        ax2_twin.plot(x, pv[start_idx:], label='Voltage [V]', color='red', linewidth=2)
        ax2_twin.plot(x, ptmp[start_idx:], label='Temperature [°C]', color='green', linewidth=2)
        ax2_twin.plot(x, pch[start_idx:], label='Charge [Ah]', color='orange', linewidth=2)
        ax2.set_ylabel('Current [A]', color='blue')
        ax2_twin.set_ylabel('Voltage [V] / Temp [°C] / Charge [Ah]', color='red')
        ax2.set_title('Power Supply Sensors')
        ax2.grid(True)

        # Combine legends from both axes
        lines1, labels1 = ax2.get_legend_handles_labels()
        lines2, labels2 = ax2_twin.get_legend_handles_labels()
        ax2.legend(lines1 + lines2, labels1 + labels2, loc='upper right')

        # Subplot 3: Airspeed
        ax3 = fig.add_subplot(4, 1, 3)
        ax3.plot(x, ahs[start_idx:], label='Airspeed [m/s]', color='purple', linewidth=2)
        ax3.set_ylabel('Airspeed [m/s]')
        ax3.set_title('Airspeed Sensor')
        ax3.grid(True)
        ax3.legend(loc='upper right')

        # Subplot 4: Tensometer
        ax4 = fig.add_subplot(4, 1, 4)
        ax4.plot(x, tens[start_idx:], label='Tensometer [g]', color='brown', linewidth=2)
        ax4.set_xlabel('Time [s] since start')
        ax4.set_ylabel('Tensometer [g]')
        ax4.set_title('Tensometer Sensor')
        ax4.grid(True)
        ax4.legend(loc='upper right')

        plt.tight_layout()

# --- Start read thread ---
thread = threading.Thread(target=read_serial)
thread.start()

# --- Real-time plot ---
fig = plt.gcf()
fig.set_size_inches(8, 6)  # Set initial window size (width=12in, height=10in)
fig.canvas.manager.set_window_title('Data Receiver - Real-time Monitoring')  # Set window title
ani = FuncAnimation(fig, update, interval=PLOT_INTERVAL_MS, cache_frame_data=False)
plt.show(block=False)

# --- Wait for input to finish ---
input("Press Enter to close...\n")
running = False

# Wait for the thread to finish
thread.join()
print("[INFO] Program ended.")
