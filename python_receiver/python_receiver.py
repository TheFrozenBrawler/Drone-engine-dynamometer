import serial
import csv
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import threading
import time
from datetime import datetime
from bisect import bisect_left

# --- Configuration ---
PORT = 'COM8'               # Arduino serial port
BAUDRATE = 9600             # baud rate
CSV_FILE = 'dane.csv'       # output CSV filename
# Time between subsequent UART reads (in seconds). Change this value
# to control the sampling rate.
SAMPLE_INTERVAL = 0.1       # seconds
# Plot refresh interval (in ms)
PLOT_INTERVAL_MS = 250      # milliseconds
PLOT_TIMESPAN = 3 * 60      # 3 minutes

# --- Data buffers ---
adc_values = []
voltages = []
timestamps = []  # time (seconds since start) for each sample
running = True  # flag to stop the program
data_lock = threading.Lock()

# --- Function to read data from UART ---
def read_serial():
    global running
    ser = serial.Serial(PORT, BAUDRATE, timeout=1)
    print(f"[INFO] Connected with {PORT}")
    # start time for relative timestamps
    start_time = time.time()

    with open(CSV_FILE, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        # write a human-readable timestamp, ADC value and voltage
        writer.writerow(['Timestamp', 'ADC_value', 'Voltage(V)'])

        while running:
            line = ser.readline().decode('utf-8').strip()
            if ';' in line:
                try:
                    adc_str, voltage_str = line.split(';')
                    adc = int(adc_str)
                    voltage = float(voltage_str)
                except ValueError:
                    print("[INFO] Incorrect line received")
                    adc = 0
                    voltage = 0

                # record timestamp: human-readable (ISO) and relative time in seconds
                iso_ts = datetime.now().isoformat(timespec='milliseconds')
                rel_ts = time.time() - start_time

                # append under lock to avoid concurrent access during plotting
                with data_lock:
                    adc_values.append(adc)
                    voltages.append(voltage)
                    timestamps.append(rel_ts)

                # write to CSV (keep this outside the lock to minimize contention)
                if isinstance(adc, int) and isinstance(voltage, float):
                    writer.writerow([iso_ts, adc, voltage])
                else:
                    # failed sample recorded as NaN
                    writer.writerow([iso_ts, "NaN", "NaN"])

    ser.close()
    print("[INFO] Connection with the port has been closed.")

# --- Function to update the plot ---
def update(frame):
    with data_lock:
        ts = timestamps.copy()
        vs = voltages.copy()

    if len(ts) > 0:
        # limit to last PLOT_TIMESPAN seconds
        end_t = ts[-1]
        start_threshold = end_t - PLOT_TIMESPAN
        start_idx = bisect_left(ts, start_threshold)

        x = ts[start_idx:]
        y = vs[start_idx:]

        plt.cla()
        plt.plot(x, y, label='Voltage [V]')
        plt.xlabel('Time [s] since start')
        plt.ylabel('Voltage [V]')
        plt.title('Odczyt napięcia z Arduino')
        plt.grid(True)
        plt.legend(loc='upper right')

# --- Start read thread ---
thread = threading.Thread(target=read_serial)
thread.start()

# --- Real-time plot ---
ani = FuncAnimation(plt.gcf(), update, interval=PLOT_INTERVAL_MS)
plt.show(block=False)

# --- Wait for input to finish ---
input("\\Press Enter to close...\\n")
running = False

# Wait for the thread to finish
thread.join()
print("[INFO] Program ended.")
