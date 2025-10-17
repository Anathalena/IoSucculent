import serial
import os
import numpy as np
import pandas as pd
from datetime import datetime
import time
import argparse

CSV_DIR = os.path.join(os.getcwd(), 'data_output')
os.makedirs(CSV_DIR, exist_ok=True)

def send_data(ser, data_bytes, delay=0.01):
    for b in data_bytes:
        ser.write(bytes([b]) if isinstance(b, int) else b)
        time.sleep(delay)

def read_data(ser):
    ser.write(b'R')
    time.sleep(0.05)
    output = ser.readlines()
    output = np.array(output)
    decoded_output = np.strings.decode(output, encoding='utf-8')
    sliced_output = np.strings.slice(decoded_output, -1)
    return sliced_output
    
def to_processed_dataframe(unprocessed_output):
    data_series = pd.Series(unprocessed_output)
    processed_df = data_series.str.split(',', expand=True).astype(int)
    processed_df.columns = ['Day', 'Month', 'Year', 'Hours', 'Minutes', 'SoilMoisture [%]', 'AirTemperature [C]', 'AirHumidity [%]']
    processed_df = processed_df[processed_df.Year!=0]
    processed_df['Datetime'] = pd.to_datetime(
    dict(year=(processed_df['Year']+2000), month=processed_df['Month'], day=processed_df['Day'],
         hour=processed_df['Hours'], minute=processed_df['Minutes'])
    )
    processed_df.drop(columns=['Day', 'Month', 'Year', 'Hours', 'Minutes'], inplace=True)
    processed_df = processed_df.sort_values(by='Datetime')
    processed_df = processed_df[['Datetime', 'SoilMoisture', 'AirTemperature', 'AirHumidity']]
    return processed_df

def main():
    parser = argparse.ArgumentParser(description="IoSucculent data processing script.")
    parser.add_argument('--port', type=str, required=True, help='Serial port device of STM32 board (e.g. \'/dev/ttyUSB0\')')
    parser.add_argument('--sync', type=bool, default=True,
                        help='Synchronise time with STM32 RTC (default: True). Use --sync=False to disable')
    parser.add_argument('--baudrate', type=int, default=115200,
                        help='UART baudrate (default: 115200)')


    args = parser.parse_args()

    ser = serial.Serial(args.port, baudrate=args.baudrate, bytesize=8, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=1)

    if args.sync:
        print("Syncing datetime...")
        curr_datetime = datetime.now()
        send_data(ser, [b'T', 
                        curr_datetime.hour, 
                        curr_datetime.minute, 
                        curr_datetime.second,
                        b'D',
                        curr_datetime.year % 100,
                        curr_datetime.month,
                        curr_datetime.day])
        
    print("Reading data..")
    raw_output = read_data(ser)
    output = to_processed_dataframe(raw_output)
    name = str(output['Datetime'].iloc[0]) + ' - ' + str(output['Datetime'].iloc[-1])
    output.to_csv(os.path.join(CSV_DIR, name), index=False)
    print("Data saved to ", name, ".csv")

if __name__ == '__main__':
    main()