#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>  // For RTC
#include <Servo.h>    // For Servo control

// DHT sensor settings
#define DHTPIN 2    // Connect to D2 pin on Arduino (Adjust as needed)
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LCD settings (I2C address 0x27, 20 columns, 4 rows)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// RTC settings
RTC_DS1307 rtc;

// Servo pin
Servo servo;
int SERVO_PIN = 9;  // Adjust for your Arduino pin

// Pins for LEDs and buzzer
int yellow = 4;    // Change to yellow LED pin
int buzzer_pin = 3;
#define blue 5
#define green 6    // Keep green LED always ON for the system
#define red 7

// Define check-in time
int checkInHour = 3;
int checkInMinute1 = 0;
int checkInMinute2 = 58;
int userCheckInHour;
int userCheckInMinute;
int checkInHour2 = 6;
int ClassHour2 = 4;
int classminute = 1;
int ClassHour1 = 12;

// Timer for class time checking
unsigned long previousMillis = 0;
const long interval = 1000;  // 1 second

void setup() {
  Serial.begin(9600);
  
  // Attach Servo
  servo.attach(SERVO_PIN);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  
  // Initialize DHT sensor
  dht.begin();
  
  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    lcd.clear();
    lcd.print(F("Couldn't find RTC"));
    while (1);
  }

  // Set up pin modes
  pinMode(yellow, OUTPUT); // Yellow LED pin for Medicine
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT); // Green LED always on
  pinMode(buzzer_pin, OUTPUT);

  // Initialize RTC time (set to compile time)
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  // Green LED stays on for system operation
  digitalWrite(green, HIGH);  // Green LED always on for system
  tone(buzzer_pin,1000);
  delay(1000);
}

void loop() {
  // Handle RTC
  rtcfun();

  // Check class time every second
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ClassTime();
  }

  // Send sensor data every 2 seconds
  sendSensor();
}

void rtcfun() {
  DateTime now = rtc.now();

  lcd.setCursor(0, 2);
  lcd.print("Date: ");
  lcd.setCursor(6, 2);
  lcd.print(now.year(), DEC);
  lcd.print(":");
  lcd.print(now.month(), DEC);
  lcd.print(":");
  lcd.print(now.day(), DEC);
  lcd.print("   ");

  lcd.setCursor(0, 3);
  lcd.print(F("Time: "));
  lcd.setCursor(6, 3);
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);
  lcd.print("   ");
}

void ClassTime() {
  DateTime now = rtc.now();
  userCheckInHour = now.hour();
  userCheckInMinute = now.minute();
  int usercheckseconds = now.second();

  if ((userCheckInHour == 14) && (userCheckInMinute == 43) && (usercheckseconds >= 5 && usercheckseconds <= 15)) {
    // Servo movement for class 1
    for (int i = 0; i < 45; i++) {
      servo.write(i);
      delay(1);
    }
    Serial.println("Class Time 1");
    lcd.setCursor(0, 0);
    lcd.print(F("kaabsool 3 xabo"));

    // Visual and audible feedback using Yellow LED for Medicine 1
    for (int i = 0; i < 8; i++) {
      tone(buzzer_pin, 1111);
      digitalWrite(red, HIGH); // Turn on yellow LED for medicine time
      delay(300);
      tone(buzzer_pin, 2222);
      digitalWrite(red, LOW);  // Turn off yellow LED
      delay(300);
      tone(buzzer_pin, 3333);
      digitalWrite(red, HIGH);
      delay(300);
      tone(buzzer_pin, 3333);
      digitalWrite(red, LOW);
      delay(300);
      noTone(buzzer_pin);
    }

  }  // end class time 1

  else if ((userCheckInHour == 14) && (userCheckInMinute == 44) && (usercheckseconds >= 5 && usercheckseconds <= 15)) {
    // Servo movement for class 2
    for (int i = 45; i < 90; i++) {
      servo.write(i);
      delay(1);
    }
    lcd.setCursor(0, 0);
    lcd.print(F("Amuudiyow 1 xabo"));
    
    // Visual and audible feedback using Yellow LED for Medicine 2
    for (int i = 0; i < 8; i++) {
      tone(buzzer_pin, 1111);
      digitalWrite(blue, HIGH); // Turn on yellow LED for medicine time
      delay(300);
      tone(buzzer_pin, 2222);
      digitalWrite(blue, LOW);  // Turn off yellow LED
      delay(300);
      tone(buzzer_pin, 3333);
      digitalWrite(blue, HIGH);
      delay(300);
      tone(buzzer_pin, 3333);
      digitalWrite(blue, LOW);
      delay(300);
      noTone(buzzer_pin);
    }
  } 

  else if ((userCheckInHour == 14) && (userCheckInMinute == 45) && (usercheckseconds >= 5 && usercheckseconds <= 15)) {
    // Servo movement for class 3
    for (int i = 90; i < 165; i++) {
      servo.write(i);
      delay(1);
    }

    lcd.setCursor(0, 0);
    lcd.print(F("banadol 2 xabo"));

    // Visual and audible feedback using Yellow LED for Medicine 3
    for (int i = 0; i < 10; i++) {
      tone(buzzer_pin, 1111);
      digitalWrite(yellow, HIGH); // Turn on yellow LED for medicine time
      delay(300);
      tone(buzzer_pin, 2222);
      digitalWrite(yellow, LOW);  // Turn off yellow LED
      delay(300);
      tone(buzzer_pin, 3333);
      digitalWrite(yellow, HIGH);
      delay(300);
      tone(buzzer_pin, 3333);
      digitalWrite(yellow, LOW);
      delay(300);
      noTone(buzzer_pin);
    }
  }
  
  // Time off, reset all LEDs
  lcd.setCursor(0, 0);
  lcd.print(F("...Time off..!   "));
  noTone(buzzer_pin);
  digitalWrite(yellow, LOW);  // Turn off yellow LED after the medicine time is over
  delay(1000);  // Wait for the next cycle
}

void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();  // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!     ");
    return;
  }

  // Print to Serial Monitor
  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print("    Humidity : ");
  Serial.println(h);

  // Display on LCD
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%    ");
}