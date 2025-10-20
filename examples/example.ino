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
