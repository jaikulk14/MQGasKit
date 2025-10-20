# MQGasKit

MQGasKit is an easy-to-use Arduino library for working with MQ series gas sensors (MQ-2, MQ-6, MQ-7, MQ-135).  
It simplifies sensor initialization, calibration, and gas concentration readings, making it useful for air quality monitoring and IoT projects.

## Features
- Supports MQ-2, MQ-6, MQ-7, MQ-135
- Simple setup and calibration
- Read gas concentration in ppm
- Example sketches included

## Installation
1. Download or clone this repository.
2. Copy the `MQGasKit` folder into your Arduino `libraries` folder.
3. Restart the Arduino IDE.

## Usage Example
```cpp
#include <MQGasKit.h>

MQGasKit sensor(A0, MQ2);  // Single sensor on A0

void setup() {
  Serial.begin(115200);
  sensor.calibrate();       // Calibrate in clean air
}

void loop() {
  // Automatically detect the gas affecting the sensor
  String gas = sensor.detectGas();
  Serial.print("Likely gas: ");
  Serial.println(gas);

  // Optionally, get PPM for a specific gas
  float ppm = sensor.getPPM("LPG");
  Serial.print("LPG concentration: ");
  Serial.println(ppm);

  delay(2000);
}

