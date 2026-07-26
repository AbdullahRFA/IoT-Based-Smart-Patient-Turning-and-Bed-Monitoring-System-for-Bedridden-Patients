#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ESP32Servo.h>

//================= PIN DEFINITIONS =================
#define DHTPIN          4
#define DHTTYPE         DHT22

#define FSR_PIN         34
#define BUZZER_PIN      13
#define SERVO_PIN       27

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64

//================= OBJECTS =================
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);
Servo bedServo;

//================= VARIABLES =================
bool isTilted = false;

unsigned long previousServoMillis = 0;
const unsigned long servoInterval = 10000;   // 10 seconds

//==================================================
// Function: Beep buzzer when patient position changes
//==================================================
void notifyShift()
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);          // Beep for 0.5 second
  digitalWrite(BUZZER_PIN, LOW);
}

//==================================================
// Function: Move servo smoothly between two angles
//==================================================
void moveServoSmoothly(int startAngle, int endAngle) 
{
  int stepDelay = 20; // Time in milliseconds between each degree. Increase to slow down.
  
  if (startAngle < endAngle) 
  {
    for (int pos = startAngle; pos <= endAngle; pos++) 
    {
      bedServo.write(pos);
      delay(stepDelay);
    }
  } 
  else 
  {
    for (int pos = startAngle; pos >= endAngle; pos--) 
    {
      bedServo.write(pos);
      delay(stepDelay);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  dht.begin();

  // OLED
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED Failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(10, 20);
  display.println("Patient Monitor");
  display.display();

  delay(2000);

  // Servo
  bedServo.setPeriodHertz(50);
  bedServo.attach(SERVO_PIN, 500, 2400);
  bedServo.write(10); // Start at 10 degrees (Flat)
}

void loop()
{
  //-------------------- Read Sensors ------------------------
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  int fsrValue = analogRead(FSR_PIN);

  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("DHT Error");
    return;
  }

  //-------------------- Servo Control ------------------------
  // Toggles the bed between 10 and 120 degrees every 10 seconds if a patient is present
  if (millis() - previousServoMillis >= servoInterval)
  {
    previousServoMillis = millis();

    if (isTilted && fsrValue > 100)
    {
      Serial.println("Returning Bed Flat (10 degrees)");

      // Move smoothly from 120 down to 10
      moveServoSmoothly(120, 10);
      notifyShift();        // Beep once

      isTilted = false;
    }
    else if (!isTilted && fsrValue > 100)
    {
      Serial.println("Tilting Bed (120 degrees)");

      // Move smoothly from 10 up to 120
      moveServoSmoothly(10, 120);
      notifyShift();

      isTilted = true;
    }
    else
    {
      Serial.println("Patient is absent - Skipping tilt");
    }
  }

  //-------------------- Alarm Logic ------------------------
  bool alarm = false;

  if (temperature > 38.0)
    alarm = true;

  if (fsrValue < 100)
    alarm = true;

  // Continuous alarm
  if (alarm)
  {
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else
  {
    digitalWrite(BUZZER_PIN, LOW);
  }

  //-------------------- OLED ------------------------
  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("PATIENT MONITOR");
  display.println("----------------");

  display.print("Temp : ");
  display.print(temperature, 1);
  display.println(" C");

  display.print("Hum  : ");
  display.print(humidity, 1);
  display.println(" %");

  display.print("FSR  : ");
  display.println(fsrValue);

  display.print("Servo: ");

  if (isTilted)
    display.println("TILT (120)");
  else
    display.println("FLAT (10)");

  display.print("Patient: ");

  if (fsrValue > 100)
    display.println("Present");
  else
    display.println("Absent");

  if (alarm)
  {
    display.println();
    display.println("*** ALERT ***");
  }

  display.display();

  //-------------------- Serial Monitor ------------------------
  Serial.print("Temperature : ");
  Serial.print(temperature);

  Serial.print(" C  Humidity : ");
  Serial.print(humidity);

  Serial.print(" %  FSR : ");
  Serial.print(fsrValue);

  Serial.print("  Servo : ");

  if (isTilted)
    Serial.print("Tilt(120)");
  else
    Serial.print("Flat(10)");

  Serial.print("  Patient: ");
  if (fsrValue > 100)
    Serial.println("Present");
  else
    Serial.println("Absent");

  delay(500);
  Serial.println("");
}