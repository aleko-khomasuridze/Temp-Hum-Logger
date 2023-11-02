# Arduino Data Logger with DS3231 RTC, DHT22 Sensor, OLED Display, and SD Card

## Author: Aleko Khomasuridze
## Date: November 1, 2023

This Arduino sketch is designed to log temperature and humidity data from a DHT22 sensor at specific times (9 AM and 6 PM) using a DS3231 RTC (Real-Time Clock). The data is displayed on an OLED screen and saved to an SD card in CSV format. The code follows Object-Oriented Programming (OOP) principles for modularity and organization.

## Hardware Components
- Arduino board (e.g., Arduino Uno)
- DS3231 RTC module
- DHT22 temperature and humidity sensor
- SSD1306-based OLED display
- SD card module

## Libraries Required
- RTClib: [https://github.com/adafruit/RTClib](https://github.com/adafruit/RTClib)
- Adafruit SSD1306: [https://github.com/adafruit/Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- DHT Sensor Library: [https://github.com/adafruit/DHT-sensor-library](https://github.com/adafruit/DHT-sensor-library)
- Arduino SD Library: [https://www.arduino.cc/en/Reference/SD](https://www.arduino.cc/en/Reference/SD)

## Wiring
- Connect the DS3231 RTC, DHT22 sensor, OLED display, and SD card module to the Arduino.
- Make sure to provide power, ground, and data connections for each component.

## Operation
- The sketch initializes the hardware components and the RTC.
- It updates and displays the temperature and humidity on the OLED screen.
- At 9 AM and 6 PM, it logs the data to an SD card in CSV format.
- The data is logged with a timestamp in the format: YYYYMMDD_HHMMSS.csv.
