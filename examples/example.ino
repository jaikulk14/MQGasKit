#include <MQGasKit.h>

// Example: Using MQ-2 sensor on A0
MQGasKit mq2(A0, MQ2);

void setup() {
  Serial.begin(9600);
  Serial.println("Calibrating MQ-2 in clean air...");
  mq2.calibrate();
  Serial.println("Calibration complete!");
}

void loop() {
  float co   = mq2.getPPM("CO");
  float lpg  = mq2.getPPM("LPG");
  float smoke = mq2.getPPM("Smoke");

  Serial.print("CO: ");
  Serial.print(co);
  Serial.print(" ppm | LPG: ");
  Serial.print(lpg);
  Serial.print(" ppm | Smoke: ");
  Serial.print(smoke);
  Serial.println(" ppm");

  delay(1000);
}
