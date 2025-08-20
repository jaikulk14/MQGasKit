#ifndef MQGasKit_h
#define MQGasKit_h

#include "Arduino.h"

enum MQType { MQ2, MQ3, MQ4, MQ5, MQ7, MQ9, MQ135 };

class MQGasKit {
  public:
    MQGasKit(uint8_t pin, MQType sensor);
    void calibrate();
    float getPPM(String gas);

  private:
    uint8_t _pin;
    MQType _sensor;
    float Ro;
    float RL = 10.0; // default load resistor 10kΩ

    float readRs();
    float getSlope(String gas);
    float getIntercept(String gas);
    float getCleanAirRatio();
};

#endif
