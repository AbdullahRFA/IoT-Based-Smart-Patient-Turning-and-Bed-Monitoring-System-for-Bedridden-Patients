//================= BLYNK DEFINITIONS =================
// These must be defined BEFORE including Blynk libraries
#define BLYNK_TEMPLATE_ID "TMPL6KmOvMUYp"
#define BLYNK_TEMPLATE_NAME "Patient Monitoring"
#define BLYNK_AUTH_TOKEN "Y22ZHZ8wqn9FvsZZpIROx8srz2p9QQer"

//================= LIBRARIES =========================
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ESP32Servo.h>

//================= WIFI CREDENTIALS ==================
char ssid[] = "S24 fe"; // Make sure this is a 2.4GHz network
char pass[] = "1234567890";

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
BlynkTimer timer;

//================= VARIABLES =================
bool isTilted = false;
bool alertNotified = false; // <-- This is the variable that was missing

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
  int stepDelay = 20; 
  
  if (startAngle < endAngle) 
  {
    for (int pos = startAngle; pos <= endAngle; pos++) 
    {
      bedServo.write(pos);
      delay(stepDelay);
      Blynk.run(); // Keep Blynk alive during long physical movements
    }
  } 
  else 
  {
    for (int pos = startAngle; pos >= endAngle; pos--) 
    {
      bedServo.write(pos);
      delay(stepDelay);
      Blynk.run(); // Keep Blynk alive
    }
  }
}

//==================================================
// Function: Main Routine (Runs every 500ms via Timer)
//==================================================
void mainMonitorRoutine()
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
  if (millis() - previousServoMillis >= servoInterval)
  {
    previousServoMillis = millis();

    if (isTilted && fsrValue > 100)
    {
      Serial.println("Returning Bed Flat (10 degrees)");
      moveServoSmoothly(90, 10);
      notifyShift();        
      isTilted = false;
    }
    else if (!isTilted && fsrValue > 100)
    {
      Serial.println("Tilting Bed (120 degrees)");
      moveServoSmoothly(10, 90);
      notifyShift();
      isTilted = true;
    }
    else
    {
      Serial.println("Patient is absent - Skipping tilt");
    }
  }

  //-------------------- Alarm Logic & Notifications -----------
  bool alarm = false;

  if (temperature > 38.0)
    alarm = true;

  if (fsrValue < 100)
    alarm = true;

  if (alarm)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    Blynk.virtualWrite(V5, "ALERT");
    
    // Trigger Blynk Push Notification ONLY ONCE per alarm
    if (!alertNotified)
    {
      // "patient_alert" is the Event Code we will create in the dashboard
      Blynk.logEvent("patient_alert", "Warning: Check on Patient!");
      alertNotified = true; 
    }
  }
  else
  {
    digitalWrite(BUZZER_PIN, LOW);
    Blynk.virtualWrite(V5, "Normal");
    
    // Reset the flag so it can notify you again next time an alarm happens
    alertNotified = false; 
  }

  //-------------------- Blynk Updates ------------------------
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, fsrValue);
  
  if (isTilted) Blynk.virtualWrite(V3, "TILT (120)");
  else Blynk.virtualWrite(V3, "FLAT (10)");

  if (fsrValue > 100) Blynk.virtualWrite(V4, "Present");
  else Blynk.virtualWrite(V4, "Absent");

  //-------------------- OLED ------------------------
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("PATIENT MONITOR");
  display.println("----------------");
  display.print("Temp : "); display.print(temperature, 1); display.println(" C");
  display.print("Hum  : "); display.print(humidity, 1); display.println(" %");
  display.print("FSR  : "); display.println(fsrValue);
  display.print("Servo: "); display.println(isTilted ? "TILT (120)" : "FLAT (10)");
  display.print("Patient: "); display.println(fsrValue > 100 ? "Present" : "Absent");
  
  if (alarm)
  {
    display.println();
    display.println("*** ALERT ***");
  }
  display.display();

  //-------------------- Serial Monitor ------------------------
  Serial.print("Temp: "); Serial.print(temperature);
  Serial.print("C  Hum: "); Serial.print(humidity);
  Serial.print("%  FSR: "); Serial.print(fsrValue);
  Serial.print("  Servo: "); Serial.print(isTilted ? "Tilt(120)" : "Flat(10)");
  Serial.print("  Patient: "); Serial.println(fsrValue > 100 ? "Present" : "Absent");
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
  display.println("Connecting WiFi...");
  display.display();

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  display.clearDisplay();
  display.setCursor(10, 20);
  display.println("Blynk Connected!");
  display.display();
  delay(1000);

  // Servo
  bedServo.setPeriodHertz(50);
  bedServo.attach(SERVO_PIN, 500, 2400);
  bedServo.write(10); // Start flat

  // Setup Timer to run the monitor routine every 500ms
  timer.setInterval(500L, mainMonitorRoutine);
}

void loop()
{
  Blynk.run();
  timer.run();
}