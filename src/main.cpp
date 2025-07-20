#include <Arduino.h> // Comment this if its uploaded using Arduino IDE
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include <EEPROM.h>

#define RECORD_INTERUPT_PIN A0 // Pin to trigger data recording, can be changed as needed
#define RECORD_SAMPLING_RATE 150 // This is records per milisecond
#define RECORD_LED_PIN 13 // Pin for the LED to indicate recording status, can be changed as needed

#define RECORD_SEQUENCE_COUNT_ADDRESS 0 // EEPROM address to store the record sequence count
#define RECORD_SEQUENCE_COUNT_MAX 255 // Maximum value for the record sequence count

#define GMT_OFFSET 7 // GMT offset in hours, can be adjusted as needed

#define SD_CS_PIN 10 // Chip select pin for SD card

Adafruit_MPU6050 mpu;
RTC_DS1307 rtc;

int currentRecordSequence;

String getFilename() {
    DateTime now = rtc.now();
    char buffer[30];
    snprintf(buffer, sizeof(buffer), "record_%04d%02d%02d_%03d.csv",
             now.year(), now.month(), now.day(),
             currentRecordSequence);
    return String(buffer);
}

String getTimestampISO() {
    DateTime now = rtc.now();

    now = now + TimeSpan(GMT_OFFSET * 3600);

    char buffer[25]; 
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02dT%02d:%02d:%02d%s",
             now.year(), now.month(), now.day(),
             now.hour(), now.minute(), now.second(),
             (GMT_OFFSET == 0) ? "Z" : "");

    if (GMT_OFFSET != 0) {
        int offsetHours = abs(GMT_OFFSET);
        char offsetBuffer[7];
        snprintf(offsetBuffer, sizeof(offsetBuffer), "%s%02d:00",
                 (GMT_OFFSET > 0) ? "+" : "-", offsetHours);
        return String(buffer) + offsetBuffer;
    }

    return String(buffer); 
}

void recordDataToSD(const String& data) {
    File file;
    String filename = getFilename();
    if (!SD.exists(filename)) {
        Serial.print("Creating ");
        Serial.println(filename);
        file = SD.open(filename, FILE_WRITE);
        file.println("Timestamp,AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ,Temp");
        file.close();
    }
    file = SD.open(filename, FILE_WRITE);
    if (file) {
        file.println(data);
        file.close();
        Serial.print("Data recorded to ");
        Serial.println(filename);
    } else {
        Serial.println("Error opening file for writing");
    }
    file.close();
}

void setup()
{
    // Intialize serial
    Serial.begin(9600);

    while (Serial.available() == 0)
    {
        delay(10);
    }

    // Intialize the MPU6050
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }

    // Set the MPU6050 settings
    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);

    Serial.println("MPU6050 Found!");

    // Inialize RTC
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        while (1)
        {
            delay(10);
        }
    } else
    {
        Serial.println("RTC found!");
    }

    if (!rtc.isrunning())
    {
        Serial.println("RTC is NOT running!");
        // Time would be set as the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    
    // Intialize SD card
    if (!SD.begin(SD_CS_PIN))
    {
        Serial.println("SD card initialization failed!");
        while (1)
        {
            delay(10);
        }
    }
    else
    {
        Serial.println("SD card initialized successfully!");
    }

    // Update record sequence 
    currentRecordSequence = EEPROM.read(RECORD_SEQUENCE_COUNT_ADDRESS);
    if (currentRecordSequence < 0 || currentRecordSequence > RECORD_SEQUENCE_COUNT_MAX)
    {
        currentRecordSequence = 0;
    }
    currentRecordSequence++;
    EEPROM.write(RECORD_SEQUENCE_COUNT_ADDRESS, currentRecordSequence);

    // Set PINs Mode
    pinMode(RECORD_INTERUPT_PIN, INPUT_PULLUP);
    pinMode(RECORD_LED_PIN, OUTPUT);
}

void loop()
{
    if (digitalRead(RECORD_INTERUPT_PIN) == LOW)
    {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        digitalWrite(RECORD_LED_PIN, HIGH); 

        char dataBuffer[100];
        snprintf(dataBuffer, sizeof(dataBuffer), "%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
            getTimestampISO().c_str(),
            a.acceleration.x, a.acceleration.y, a.acceleration.z,
            g.gyro.x, g.gyro.y, g.gyro.z,
            temp.temperature);

        recordDataToSD(String(dataBuffer));
        
        digitalWrite(RECORD_LED_PIN, LOW);

        delay(RECORD_SAMPLING_RATE);
    }
}
