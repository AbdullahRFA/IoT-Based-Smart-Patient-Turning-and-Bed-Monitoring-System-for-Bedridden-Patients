#include <Wire.h>

void setup() {
  Wire.begin(21, 22);
  Serial.begin(115200);

  Serial.println("I2C Scanner");
}

void loop() {
  byte error, address;
  int count = 0;

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Found I2C device at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      count++;
    }
  }

  if (count == 0)
    Serial.println("No I2C devices found.");

  delay(5000);
}