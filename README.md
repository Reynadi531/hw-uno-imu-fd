# Arduino Uno Motion Tracking Belt - Fall Detection System

A motion tracking belt system using Arduino Uno for fall detection, featuring IMU sensors, data logging, and real-time monitoring capabilities.

## 🔧 Hardware Components

- **Arduino Uno** - Main microcontroller
- **MPU6050** - 6-axis IMU sensor (accelerometer + gyroscope)
- **DS1307 RTC Module** - Real-time clock for timestamping
- **SD Card Module** - Data logging storage
- **LED** - Recording status indicator
- **Push Button/Switch** - Data recording trigger
- **Power Supply** - Battery pack or USB power

## 📋 Features

- **Real-time Motion Data Collection**: Captures 3-axis accelerometer and gyroscope data
- **Temperature Monitoring**: Built-in temperature sensor readings
- **Data Logging**: Automatic CSV file generation with timestamped data
- **Customizable Sampling Rate**: Configurable recording frequency (default: 150ms)
- **File Management**: Automatic file naming with date and sequence numbering
- **GMT Timezone Support**: Configurable timezone offset for accurate timestamping
- **Visual Feedback**: LED indicator for recording status
- **Persistent Storage**: EEPROM-based sequence counting

## 🔌 Wiring Diagram

| Component | Arduino Pin | Notes |
|-----------|-------------|-------|
| MPU6050 VCC | 5V | Power supply |
| MPU6050 GND | GND | Ground |
| MPU6050 SDA | A4 | I2C Data |
| MPU6050 SCL | A5 | I2C Clock |
| MPU6050 AD0 | 3.3V | Sets I2C address to 0x69 |
| DS1307 VCC | 5V | RTC Power |
| DS1307 GND | GND | Ground |
| DS1307 SDA | A4 | I2C Data (shared) |
| DS1307 SCL | A5 | I2C Clock (shared) |
| SD Module VCC | 5V | SD Card Power |
| SD Module GND | GND | Ground |
| SD Module CS | Pin 10 | Chip Select |
| SD Module MOSI | Pin 11 | SPI Data Out |
| SD Module MISO | Pin 12 | SPI Data In |
| SD Module SCK | Pin 13 | SPI Clock |
| Record Button | Pin A0 | Trigger with pullup |
| Status LED | Pin 13 | Recording indicator |

## ⚙️ Configuration

### Key Constants (configurable in `main.cpp`)

```cpp
#define RECORD_INTERUPT_PIN A0        // Recording trigger pin
#define RECORD_SAMPLING_RATE 150      // Sampling interval (ms)
#define RECORD_LED_PIN 13             // Status LED pin
#define GMT_OFFSET 7                  // Timezone offset (hours)
#define SD_CS_PIN 10                  // SD card chip select pin
#define MPU6050_ADDRESS 0x69          // IMU I2C address
```

### MPU6050 Settings
- **Accelerometer Range**: ±16G
- **Gyroscope Range**: ±2000°/s
- **Filter Bandwidth**: 94Hz

## 📊 Data Format

The system generates CSV files with the following structure:

```csv
Timestamp,AccelX,AccelY,AccelZ,GyroX,GyroY,GyroZ,Temp
2024-01-15T10:30:25+07:00,-0.12,0.98,-0.05,0.01,-0.02,0.03,23.45
```

### File Naming Convention
- Format: `record_YYYYMMDD_XXX.csv`
- Example: `record_20240115_001.csv`

## 🚀 Getting Started

### Prerequisites

1. **Hardware Setup**: Wire components according to the wiring diagram
2. **Software Requirements**:
   - [PlatformIO IDE](https://platformio.org/) or Arduino IDE
   - Required libraries (automatically installed via PlatformIO)

### Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/Reynadi531/hw-uno-imu-fd.git
   cd hw-uno-imu-fd
   ```

2. **Install dependencies** (PlatformIO):
   ```bash
   pio lib install
   ```

3. **Build and upload**:
   ```bash
   pio run --target upload
   ```

### First Run Setup

1. **RTC Initialization**: On first power-up, the RTC will be set to compilation time
2. **SD Card**: Insert a formatted SD card before powering on
3. **Testing**: Monitor serial output at 9600 baud for system status

## 📚 Library Dependencies

- `Adafruit MPU6050` (v2.2.6+) - IMU sensor interface
- `RTClib` (v2.1.4+) - Real-time clock functionality  
- `SD` (v1.3.0+) - SD card file operations
- `Adafruit Sensor` - Unified sensor library
- `Wire` - I2C communication
- `EEPROM` - Non-volatile storage

## 🔍 Usage

### Normal Operation
1. Power on the device
2. Wait for initialization (check serial monitor)
3. Press and hold the record button to start data collection
4. LED will blink during recording
5. Data is automatically saved to SD card

### Data Retrieval
1. Power off the device
2. Remove SD card
3. Insert into computer to access CSV files
4. Import data into analysis software

## ⚡ Power Management

- **Typical Current Draw**: ~50-100mA during operation
- **Battery Life**: Depends on usage pattern and battery capacity
- **Power Saving**: Device only records when trigger is active

## 🛠️ Troubleshooting

### Common Issues

| Problem | Possible Cause | Solution |
|---------|---------------|----------|
| MPU6050 not found | Wiring/I2C address | Check connections, verify AD0 pin |
| RTC not found | Wiring/power | Check I2C connections and power |
| SD card failed | Card format/wiring | Use FAT32 format, check SPI pins |
| No data recording | Button wiring | Check pullup resistor configuration |

### Serial Monitor Messages
- `MPU6050 Found!` - IMU initialized successfully
- `RTC found!` - Real-time clock working
- `SD card initialized successfully!` - Storage ready
- `Creating record_YYYYMMDD_XXX.csv` - New file created
- `Data recorded to...` - Successful data write

## 🔮 Fall Detection Algorithm (Future Enhancement)

This hardware platform is designed to support fall detection algorithms that could analyze:

- **Acceleration Patterns**: Sudden changes indicating falls
- **Gyroscope Data**: Rotational movement analysis  
- **Threshold Detection**: Configurable sensitivity levels
- **Machine Learning**: Pattern recognition for improved accuracy

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## ✨ Acknowledgments

- Adafruit for excellent sensor libraries
- Arduino community for extensive documentation
- Contributors to the open-source libraries used

## 📞 Support

For questions, issues, or contributions:
- Open an issue on GitHub
- Check the troubleshooting section
- Review the hardware connections

---

**Note**: This is a research/educational project. For production fall detection systems, additional safety measures and testing are required.