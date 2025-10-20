#ifndef MQGASKIT_H
#define MQGASKIT_H

#include <Arduino.h>

enum MQType { MQ2, MQ3, MQ4, MQ5, MQ7, MQ9, MQ135 };

class MQGasKit {
public:
    // Single sensor constructor
    MQGasKit(uint8_t pin, MQType sensor);

    // Double sensor constructor
    MQGasKit(uint8_t pin1, MQType sensor1, uint8_t pin2, MQType sensor2);

    void calibrate();                // Calibrate Ro(s) in clean air
    float readRs(uint8_t index=0);   // Read Rs from sensor index 0 or 1
    float getPPM(String gas, uint8_t index=0); // PPM for specific gas, sensor index
    String detectGas(uint8_t index=0); // Detect likely gas for sensor index

private:
    uint8_t _pins[2];
    MQType _sensors[2];
    float Ro[2];                     // Baseline resistances
    const float RL = 10.0;           // Load resistor in kilo-ohms
    uint8_t sensorCount;

    float getCleanAirRatio(uint8_t index);
    float getSlope(String gas, uint8_t index);
    float getIntercept(String gas, uint8_t index);
};

#endif
