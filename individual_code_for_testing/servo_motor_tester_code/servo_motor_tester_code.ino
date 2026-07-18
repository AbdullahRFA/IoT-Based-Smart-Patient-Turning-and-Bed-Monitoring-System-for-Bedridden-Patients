#include <ESP32Servo.h>

Servo myServo;

const int servoPin = 18;

void setup() {
  Serial.begin(115200);

  myServo.setPeriodHertz(50);          // Standard servo frequency
  myServo.attach(servoPin, 500, 2400); // Min and max pulse width

  Serial.println("MG996R Servo Test");
}

void loop() {
  Serial.println("0°");
  myServo.write(0);
  delay(2000);

  Serial.println("90°");
  myServo.write(90);
  delay(2000);

  Serial.println("180°");
  myServo.write(180);
  delay(2000);
}

// #include <ESP32Servo.h>

// Servo myServo;

// const int servoPin = 18;

// void setup() {
//   myServo.setPeriodHertz(50);
//   myServo.attach(servoPin, 500, 2400);
// }

// void loop() {

//   for (int pos = 0; pos <= 180; pos++) {
//     myServo.write(pos);
//     delay(15);
//   }

//   for (int pos = 180; pos >= 0; pos--) {
//     myServo.write(pos);
//     delay(15);
//   }

// }