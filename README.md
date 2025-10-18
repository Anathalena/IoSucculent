## IoSucculent

This is a simple plant monitoring system that collects information about soil moisture, ambient temperature, and humidity. One can use the provided script `process_data.py` to collect gathered data from the controller and save it to a CSV file using:

```
python process_data.py --port=<port>
```

`<port>` information can be found in `Device Manager` (for Windows) or by using `dmesg | grep tty` (for Linux)

It is also possible to use `process_data.py` to synchronise RTC datetime (enabled by default, to disable pass `--sync=False`)

Sensor data is logged into a circular buffer (capacity of `1024` logs) together with datetime information. 
Data formatting and sorting are handled on the Python side. 
Data acquisition and transmission logic is in `Core/Inc/sensors.h`/`Core/Src/sensors.c`. 

![Realisation](/iosucc2.jpeg?raw=true)


### Hardware
- development board: [MINI-M4](https://download.mikroe.com/documents/starter-boards/mini/stm32/f4/mini-m4-stm32-manual-v100.pdf) (containing STM32F415RG)
- debugger/programmer: [ST-LINK/V2](https://www.st.com/en/development-tools/st-link-v2.html#overview) 
- ambient temperature/humidity sensor: [BME280](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf)
- capacitive soil moisture sensor
- USB to UART converter: [TTL-232R-3V3](https://ftdichip.com/products/ttl-232r-3v3/)

![Hardware scheme](/iosucc.jpeg?raw=true)

### Software
- IDE: STM32CubeIDE
- BME280 driver: [ciastkolog/BMP280_STM32](https://github.com/ciastkolog/BMP280_STM32/tree/master)
    - Located in `Core/Inc/bmp280.h`/`Core/Src/bmp280.c` 

### Used peripherals
- **RTC**: Alram A is configured to trigger every hour. 
    - In the callback, data is collected and logged (together with the current datetime) to a circular buffer.
- **ADC1**: Used for soil moisture sensor (12bit resolution) 
- **I2C1**: Used for communication with BME280. 
    - Device address is `1110110 (0x76)`
- **USART2**: Configured in interrupt mode with baudrate `115200 Bits/s`, `8bit` word length and no parity
    - Controller expects from user (in this case, Python) to either send:
     - `'R'` character, after which it will send back all data from the circular buffer
  - `'T' or 'D'` characters after which the controller will expect three bytes for Hour/Minute/Second (in `'T'` case) or Year/Month/Day (for `'D'` case). After receiving data for RTC datetime synchronization, it will go back to expecting a single character.
  - All other input will result in an error message.
 
### Clock configuration
- Using external `32.768kHz` clock
