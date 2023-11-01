/*
  Arduino Data Logger with DS3231 RTC, DHT22 Sensor, OLED Display, and SD Card
  Author: Your Name
  Date: November 1, 2023

  This Arduino sketch is designed to log temperature and humidity data from a DHT22 sensor
  at specific times (9 AM and 6 PM) using a DS3231 RTC (Real-Time Clock). The data is
  displayed on an OLED screen and saved to an SD card in CSV format. The code follows
  Object-Oriented Programming (OOP) principles for modularity and organization.

  Hardware Components:
  - Arduino board (e.g., Arduino Uno)
  - DS3231 RTC module
  - DHT22 temperature and humidity sensor
  - SSD1306-based OLED display
  - SD card module

  Libraries Required:
  - RTClib: https://github.com/adafruit/RTClib
  - Adafruit SSD1306: https://github.com/adafruit/Adafruit_SSD1306
  - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
  - Arduino SD Library: https://www.arduino.cc/en/Reference/SD

  Wiring:
  - Connect the DS3231 RTC, DHT22 sensor, OLED display, and SD card module to the Arduino.
  - Make sure to provide power, ground, and data connections for each component.

  Operation:
  - The sketch initializes the hardware components and the RTC.
  - It updates and displays the temperature and humidity on the OLED screen.
  - At 9 AM and 6 PM, it logs the data to an SD card in CSV format.
  - The data is logged with a timestamp in the format: YYYYMMDD_HHMMSS.csv.

  @author: Aleko Khomasuridze
  @date: November 1, 2023
*/


#include <Wire.h>                 // library for two wire communication
#include <RTClib.h>               // library for real time clock
#include <DHT.h>                  // library for temperature and humidity sensor
#include <Adafruit_SSD1306.h>     // library for OLED Chip driver
#include <Adafruit_GFX.h>         // library for graphics 
#include <SPI.h>                  // library for seria peripherial interface
#include <SD.h>                   // library for writing data on SD card

// Definitions for the DHT sensor
#define DHT_PIN 2
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

// Definitions for the DS3231 RTC
RTC_DS3231 rtc;

// Definitions for the OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definitions for SD card
const int chipSelect = 10;

class Logger {
public:
  Logger() : lastLoggedHour(-1) {}

  void begin() {
    Serial.begin(9600);
    dht.begin();
    Wire.begin();
    rtc.begin();

    if (!rtc.isrunning()) {
      Serial.println("RTC is not running. Setting time...");
      rtc.adjust(DateTime(2023, 11, 1, 9, 0, 0));
    }

    if (!display.begin(SSD1306_I2C_ADDRESS, OLED_RESET)) {
      Serial.println(F("SSD1306 allocation failed"));
      for (;;);
    }
    display.display();
    delay(2000);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Temperature:");
    display.println("Humidity:");
    display.display();

    // Initialize the SD card
    if (!SD.begin(chipSelect)) {
      Serial.println("SD card initialization failed!");
      while (1);
    }
  }

  void logData() {
    // Read the current time from the RTC
    DateTime now = rtc.now();

    // Read temperature and humidity from the sensor
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Update the OLED display with the current temperature and humidity
    display.setCursor(80, 0);
    display.println(temperature, 1);
    display.setCursor(80, 10);
    display.println(humidity, 1);
    display.display();

    // Check if it's the specified logging time (9 AM and 6 PM)
    if ((now.hour() == 9 && now.minute() == 0) || (now.hour() == 18 && now.minute() == 0)) {
      if (now.hour() != lastLoggedHour) {
        logToSDCard(temperature, humidity, now);
        lastLoggedHour = now.hour();
      }
    }
  }

  void logToSDCard(float temperature, float humidity, DateTime timestamp) {
    // Create a new file with a unique name based on the date
    String fileName = "Log_" + formatDate(timestamp) + ".csv";
    File dataFile = SD.open(fileName, FILE_WRITE);

    // Write temperature and humidity data to the file
    if (dataFile) {
      dataFile.print(formatDateTime(timestamp));
      dataFile.print(",");
      dataFile.print(temperature, 1);
      dataFile.print(",");
      dataFile.print(humidity, 1);
      dataFile.println();
      dataFile.close();
      Serial.println("Data logged to SD card.");
    } else {
      Serial.println("Error opening data file.");
    }
  }

private:
  int lastLoggedHour;

  String formatDateTime(DateTime dt) {
    char dateTime[20];
    snprintf(dateTime, sizeof(dateTime), "%04d-%02d-%02d %02d:%02d:%02d", dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second());
    return String(dateTime);
  }

  String formatDate(DateTime dt) {
    char date[11];
    snprintf(date, sizeof(date), "%04d%02d%02d", dt.year(), dt.month(), dt.day());
    return String(date);
  }
};

Logger dataLogger;

void setup() {
  dataLogger.begin();
}

void loop() {
  dataLogger.logData();
  delay(60000); // Delay for 1 minute (adjust as needed)
}
