/# 
Adafruit GFX Library
Adafruit SSD1306
DHT sensor library
HX711
ESP32Servo
#/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include "HX711.h"
#include <ESP32Servo.h> // Library for Servo control on ESP32

// Pin Definitions based on your table
#define DHTPIN 4
#define DHTTYPE DHT22
#define BUZZER_PIN 13
#define LOADCELL_DOUT_PIN 18
#define LOADCELL_SCK_PIN 19
#define SERVO_PIN 27 // Signal(PWM) pin from your table

// OLED Configuration (I2C)
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
// SCL is GPIO 21, SDA is GPIO 22 as per your table
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

DHT dht(DHTPIN, DHTTYPE);
HX711 scale;
Servo bedServo;

// Variables for logic
unsigned long lastMoveTime = 0;
const unsigned long moveInterval = 10000; // Demo: 10 seconds (Change for real use)
bool isTilted = false;

void setup() {
  Serial.begin(115200);
  
  // Initialize Sensors & Actuators
  dht.begin();
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initialize Servo
  bedServo.attach(SERVO_PIN);
  bedServo.write(0); // Set initial position (flat)
  
  // Initialize OLED on I2C pins 21 (SCL) and 22 (SDA)
  Wire.begin(22, 21); // Wire.begin(SDA, SCL)
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("OLED Connection Failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Patient Monitoring");
  display.println("System Initializing...");
  display.display();
  delay(2000);
}

void loop() {
  // Reading environmental data
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();
  
  // Reading weight/pressure data
  long raw_weight = 0;
  if (scale.is_ready()) {
    raw_weight = scale.read(); 
  }

  // Automatic Bed Tilt Logic
  // If the patient is immobile for too long, tilt the bed
  if (millis() - lastMoveTime > moveInterval) {
    if (!isTilted) {
      bedServo.write(30); // Tilt to 30 degrees
      isTilted = true;
    } else {
      bedServo.write(0);  // Return to flat
      isTilted = false;
    }
    lastMoveTime = millis();
  }

  // Displaying data on OLED
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("PATIENT MONITOR");
  display.println("---------------------");
  display.print("Temp: "); display.print(temp); display.println(" C");
  display.print("Hum:  "); display.print(humidity); display.println(" %");
  display.print("Load: "); display.println(raw_weight);
  display.print("Tilt Status: "); display.println(isTilted ? "Active" : "Flat");
  
  // Alert Logic
  if (temp > 38.0 || (isTilted && temp > 35.0)) { 
    digitalWrite(BUZZER_PIN, HIGH);
    display.setCursor(0, 55);
    display.print("!! CHECK PATIENT !!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
  
  display.display();
  delay(2000); 
}