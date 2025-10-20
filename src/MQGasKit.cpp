#include "MQGasKit.h"
#include <math.h>

// ---- Constructors ----
MQGasKit::MQGasKit(uint8_t pin, MQType sensor) {
    _pins[0] = pin;
    _sensors[0] = sensor;
    Ro[0] = 10.0;
    sensorCount = 1;
}

MQGasKit::MQGasKit(uint8_t pin1, MQType sensor1, uint8_t pin2, MQType sensor2) {
    _pins[0] = pin1; _sensors[0] = sensor1; Ro[0] = 10.0;
    _pins[1] = pin2; _sensors[1] = sensor2; Ro[1] = 10.0;
    sensorCount = 2;
}

// ---- Read Sensor Resistance ----
float MQGasKit::readRs(uint8_t index) {
    int val = analogRead(_pins[index]);
    if (val == 0) val = 1;
    return (1023.0 - val) * RL / val;
}

// ---- Calibration ----
float MQGasKit::getCleanAirRatio(uint8_t index) {
    switch(_sensors[index]) {
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
    for (uint8_t i = 0; i < sensorCount; i++) {
        float rs = readRs(i);
        Ro[i] = rs / getCleanAirRatio(i);
    }
}

// ---- Slope & Intercept ----
float MQGasKit::getSlope(String gas, uint8_t index) {
    MQType s = _sensors[index];
    if (s == MQ2 && gas == "LPG") return -0.45;
    if (s == MQ2 && gas == "CO") return -0.35;
    if (s == MQ2 && gas == "Smoke") return -0.43;
    if (s == MQ7 && gas == "CO") return -0.42;
    if (s == MQ135 && gas == "CO2") return -0.38;
    if (s == MQ135 && gas == "NH3") return -0.45;
    return -0.4;
}

float MQGasKit::getIntercept(String gas, uint8_t index) {
    MQType s = _sensors[index];
    if (s == MQ2 && gas == "LPG") return 1.3;
    if (s == MQ2 && gas == "CO") return 1.0;
    if (s == MQ2 && gas == "Smoke") return 1.1;
    if (s == MQ7 && gas == "CO") return 1.7;
    if (s == MQ135 && gas == "CO2") return 2.3;
    if (s == MQ135 && gas == "NH3") return 1.5;
    return 1.0;
}

// ---- PPM Calculation ----
float MQGasKit::getPPM(String gas, uint8_t index) {
    float rs = readRs(index);
    float ratio = rs / Ro[index];
    float m = getSlope(gas, index);
    float b = getIntercept(gas, index);
    return pow(10, (log10(ratio) - b) / m);
}

// ---- Detect Gas ----
String MQGasKit::detectGas(uint8_t index) {
    float rs = readRs(index);
    float ratio = rs / Ro[index];
    String likelyGas = "Unknown";
    float minDiff = 1000.0;

    String gases[5];
    int gasCount = 0;

    MQType s = _sensors[index];
    if (s == MQ2) { gases[0]="LPG"; gases[1]="CO"; gases[2]="Smoke"; gasCount=3; }
    if (s == MQ7) { gases[0]="CO"; gasCount=1; }
    if (s == MQ135) { gases[0]="CO2"; gases[1]="NH3"; gasCount=2; }

    for (int i = 0; i < gasCount; i++) {
        float expectedRatio = pow(10, getSlope(gases[i], index) * log10(100) + getIntercept(gases[i], index));
        float diff = abs(ratio - expectedRatio);
        if (diff < minDiff) {
            minDiff = diff;
            likelyGas = gases[i];
        }
    }
    return likelyGas;
}
