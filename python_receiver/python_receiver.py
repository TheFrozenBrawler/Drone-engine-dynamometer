import serial
import csv
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import threading
import time
from datetime import datetime

# --- Configuration ---
PORT = 'COM8'               # Arduino serial port
BAUDRATE = 9600             # baud rate
CSV_FILE = 'dane.csv'       # output CSV filename
# Time between subsequent UART reads (in seconds). Change this value
# to control the sampling rate.
SAMPLE_INTERVAL = 0.1       # seconds
# Plot refresh interval (in ms)
PLOT_INTERVAL_MS = 250      # milliseconds

# --- Data buffers ---
adc_values = []
voltages = []
timestamps = []  # time (seconds since start) for each sample
running = True  # flag to stop the program

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
                    # record timestamp: human-readable (ISO) and relative time in seconds
                    iso_ts = datetime.now().isoformat(timespec='milliseconds')
                    rel_ts = time.time() - start_time

                    adc_values.append(adc)
                    voltages.append(voltage)
                    timestamps.append(rel_ts)
                    writer.writerow([iso_ts, adc, voltage])
                except ValueError:
                    print("[INFO] Incorrect line received")

                    # append placeholder values for this failed sample
                    timestamps.append(rel_ts)
                    adc_values.append(0)
                    voltages.append(0)

                    adc = "NaN"
                    voltage = "NaN"
                    # record timestamp for the failed sample (ISO and relative seconds)
                    iso_ts = datetime.now().isoformat(timespec='milliseconds')
                    rel_ts = time.time() - start_time

                    writer.writerow([iso_ts, adc, voltage])

    ser.close()
    print("[INFO] Connection with the port has been closed.")

# --- Function to update the plot ---
def update(frame):
    if len(voltages) > 0:
        plt.cla()
        plt.plot(voltages, label='Voltage [V]')
        plt.xlabel('Próbka')
        plt.ylabel('Napięcie [V]')
        plt.title('Odczyt napięcia z Arduino')
        plt.grid(True)
        plt.legend(loc='upper right')

# --- Start read thread ---
thread = threading.Thread(target=read_serial)
thread.start()

# --- Real-time plot ---
ani = FuncAnimation(plt.gcf(), update, interval=500)
plt.show(block=False)

# --- Wait for input to finish ---
input("\\Press Enter to close...\\n")
running = False

# Wait for the thread to finish
thread.join()
print("[INFO] Program ended.")
