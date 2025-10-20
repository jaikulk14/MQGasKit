#include "MQGasKit.h"
#include <math.h>

MQGasKit::MQGasKit(uint8_t pins[], MQType sensors[], uint8_t count) {
  _count = count;
  for (uint8_t i = 0; i < count; i++) {
    _pins[i] = pins[i];
    _sensors[i] = sensors[i];
    Ro[i] = 10.0;
  }
}

float MQGasKit::readRs(uint8_t index) {
  int val = analogRead(_pins[index]);
  if (val == 0) val = 1;
  return (1023.0 - val) * RL / val;
}

float MQGasKit::getCleanAirRatio(MQType sensor) {
  switch(sensor) {
    case MQ2: return 9.83;
    case MQ3: return 60.0;
    case MQ4: return 4.4;
    case MQ5: return 6.5;
    case MQ7: return 27.5;
    case MQ9: return 9.6;
    case MQ135: return 3.6;
  }
  return 10.0;
}

void MQGasKit::calibrate() {
  for (uint8_t i = 0; i < _count; i++) {
    float rs = readRs(i);
    Ro[i] = rs / getCleanAirRatio(_sensors[i]);
  }
}

float MQGasKit::getSlope(String gas, MQType sensor) {
  if (sensor == MQ2 && gas == "LPG") return -0.45;
  if (sensor == MQ2 && gas == "CO") return -0.35;
  if (sensor == MQ2 && gas == "Smoke") return -0.43;
  if (sensor == MQ7 && gas == "CO") return -0.42;
  if (sensor == MQ135 && gas == "CO2") return -0.38;
  if (sensor == MQ135 && gas == "NH3") return -0.45;
  if (sensor == MQ3 && gas == "Alcohol") return -0.42;
  return -0.4;
}

float MQGasKit::getIntercept(String gas, MQType sensor) {
  if (sensor == MQ2 && gas == "LPG") return 1.3;
  if (sensor == MQ2 && gas == "CO") return 1.0;
  if (sensor == MQ2 && gas == "Smoke") return 1.1;
  if (sensor == MQ7 && gas == "CO") return 1.7;
  if (sensor == MQ135 && gas == "CO2") return 2.3;
  if (sensor == MQ135 && gas == "NH3") return 1.5;
  if (sensor == MQ3 && gas == "Alcohol") return 1.2;
  return 1.0;
}

float MQGasKit::getPPM(String gas, uint8_t index) {
  float rs = readRs(index);
  float ratio = rs / Ro[index];
  float m = getSlope(gas, _sensors[index]);
  float b = getIntercept(gas, _sensors[index]);
  return pow(10, (log10(ratio) - b) / m);
}

String MQGasKit::detectGas(uint8_t index) {
  MQType s = _sensors[index];
  String possibleGases[4];
  uint8_t count = 0;

  if (s == MQ2) { possibleGases[0] = "LPG"; possibleGases[1] = "CO"; possibleGases[2] = "Smoke"; count = 3; }
  else if (s == MQ3) { possibleGases[0] = "Alcohol"; count = 1; }
  else if (s == MQ7) { possibleGases[0] = "CO"; count = 1; }
  else if (s == MQ135) { possibleGases[0] = "CO2"; possibleGases[1] = "NH3"; count = 2; }

  float lowestPPM = 1e9;
  String bestGas = "Unknown";

  for (uint8_t i = 0; i < count; i++) {
    float ppm = getPPM(possibleGases[i], index);
    if (ppm < lowestPPM && ppm > 0) {
      lowestPPM = ppm;
      bestGas = possibleGases[i];
    }
  }
  return bestGas;
}
