#include "MQGasKit.h"
#include <math.h>

MQGasKit::MQGasKit(uint8_t pin, MQType sensor) {
  _pin = pin;
  _sensor = sensor;
  Ro = 10.0;
}

float MQGasKit::readRs() {
  int val = analogRead(_pin);
  if (val == 0) val = 1; // avoid division by zero
  return (1023.0 - val) * RL / val;
}

float MQGasKit::getCleanAirRatio() {
  switch(_sensor) {
    case MQ2:   return 9.83;
    case MQ3:   return 60.0;
    case MQ4:   return 4.4;
    case MQ5:   return 6.5;
    case MQ7:   return 27.5;
    case MQ9:   return 9.6;
    case MQ135: return 3.6;
  }
  return 10.0;
}

void MQGasKit::calibrate() {
  float rs = readRs();
  Ro = rs / getCleanAirRatio();
}

float MQGasKit::getSlope(String gas) {
  // slopes from datasheet curves (approximate)
  if (_sensor == MQ2 && gas == "LPG") return -0.45;
  if (_sensor == MQ2 && gas == "CO") return -0.35;
  if (_sensor == MQ2 && gas == "Smoke") return -0.43;
  if (_sensor == MQ7 && gas == "CO") return -0.42;
  if (_sensor == MQ135 && gas == "CO2") return -0.38;
  if (_sensor == MQ135 && gas == "NH3") return -0.45;
  return -0.4; // fallback
}

float MQGasKit::getIntercept(String gas) {
  if (_sensor == MQ2 && gas == "LPG") return 1.3;
  if (_sensor == MQ2 && gas == "CO") return 1.0;
  if (_sensor == MQ2 && gas == "Smoke") return 1.1;
  if (_sensor == MQ7 && gas == "CO") return 1.7;
  if (_sensor == MQ135 && gas == "CO2") return 2.3;
  if (_sensor == MQ135 && gas == "NH3") return 1.5;
  return 1.0; // fallback
}

float MQGasKit::getPPM(String gas) {
  float rs = readRs();
  float ratio = rs / Ro;
  float m = getSlope(gas);
  float b = getIntercept(gas);
  return pow(10, (log10(ratio) - b) / m);
}
