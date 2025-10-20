#ifndef MQGasKit_h
#define MQGasKit_h

#include "Arduino.h"

enum MQType { MQ2, MQ3, MQ4, MQ5, MQ7, MQ9, MQ135 };

class MQGasKit {
public:
    MQGasKit(uint8_t pins[], MQType sensors[], uint8_t count);
    void calibrate();
    float getPPM(String gas, uint8_t index);
    String detectGas(uint8_t index);
    float readRs(uint8_t index);

private:
    uint8_t _pins[8];          // up to 8 sensors
    MQType _sensors[8];
    float Ro[8];
    uint8_t _count;
    const float RL = 10.0;

    float getSlope(String gas, MQType sensor);
    float getIntercept(String gas, MQType sensor);
    float getCleanAirRatio(MQType sensor);
};

#endif
