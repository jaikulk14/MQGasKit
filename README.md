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

MQGasKit mq2(A0, "MQ-2");

void setup() {
  Serial.begin(9600);
  mq2.calibrate();
}

void loop() {
  float ppm = mq2.readPPM();
  Serial.print("Gas Concentration: ");
  Serial.println(ppm);
  delay(1000);
}
