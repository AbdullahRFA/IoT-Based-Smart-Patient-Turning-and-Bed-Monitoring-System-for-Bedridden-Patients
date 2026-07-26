const int fsrPin = 34;   // ADC pin

void setup() {
  Serial.begin(115200);
}

void loop() {
  int value = analogRead(fsrPin);

  Serial.print("FSR Value: ");
  Serial.println(value);

  delay(200);
}