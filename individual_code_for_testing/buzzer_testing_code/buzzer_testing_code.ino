#define BUZZER_PIN 25

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  digitalWrite(BUZZER_PIN, HIGH);  // Buzzer ON
  delay(1000);

  digitalWrite(BUZZER_PIN, LOW);   // Buzzer OFF
  delay(1000);
}