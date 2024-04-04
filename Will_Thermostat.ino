#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_AHTX0.h>
#include <Fonts/FreeSans9pt7b.h>

#define FAN 6
#define COOL 24
#define HEAT 25

#define FAN_SWITCH  13
#define REST_SWITCH 11
#define HEAT_SWITCH 10
#define COOL_SWITCH 12

#define TEMP_UP 7
#define TEMP_DOWN 8

int FanVal = LOW;
int RestVal = LOW;
int HeatVal = LOW;
int CoolVal = LOW;
int TempIn = HIGH;
int TempDe = HIGH;

double setpoint = 24.0;

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

Adafruit_AHTX0 temp_sensor;

void setup() {
  Serial.begin(115200);
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(FAN, OUTPUT);
  pinMode(COOL, OUTPUT);
  pinMode(HEAT, OUTPUT);

  pinMode(FAN_SWITCH, INPUT_PULLDOWN);
  pinMode(HEAT_SWITCH, INPUT_PULLDOWN);
  pinMode(COOL_SWITCH, INPUT_PULLDOWN);
  pinMode(REST_SWITCH, INPUT_PULLDOWN);

  pinMode(TEMP_UP, INPUT_PULLUP);
  pinMode(TEMP_DOWN, INPUT_PULLUP);

  display.begin(0x3C, true); // Address 0x3C default
  display.clearDisplay();
  display.display();

  display.setRotation(1);
  display.setFont(&FreeSans9pt7b);

  if (temp_sensor.begin()) {
    Serial.println("Found AHT20");
  } else {
    Serial.println("Didn't find AHT20");
  }  

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
}

// the loop function runs over and over again forever
void loop() {
  
  display.clearDisplay();
  sensors_event_t humidity, temp;
  
  temp_sensor.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  display.setCursor(0,20); 
  display.print("Set: "); display.print(setpoint); display.println(" C");
  display.setCursor(0,40);
  display.print("Temp: "); display.print(temp.temperature); display.println(" C");
  display.setCursor(0,60);
  display.print("Hum: "); display.print(humidity.relative_humidity); display.println(" %");
  Serial.print("Temperature: ");Serial.print(temp.temperature);Serial.println(" degrees C");
  Serial.print("Pressure: ");Serial.print(humidity.relative_humidity);Serial.println(" RH %");
  
  yield();
  display.display();
  delay(100);


  FanVal = digitalRead(FAN_SWITCH);
  RestVal = digitalRead(REST_SWITCH);
  HeatVal = digitalRead(HEAT_SWITCH);
  CoolVal = digitalRead(COOL_SWITCH);
  TempIn = digitalRead(TEMP_UP);
  TempDe = digitalRead(TEMP_DOWN);

  if (HeatVal == HIGH && temp.temperature <= setpoint) {
  
    digitalWrite(FAN, HIGH);
    digitalWrite(HEAT, HIGH);
    digitalWrite(COOL, LOW);

  } else if (CoolVal == HIGH && temp.temperature >= setpoint) {
    digitalWrite(FAN, HIGH);
    digitalWrite(COOL, HIGH);
    digitalWrite(HEAT, LOW);

  } else if (FanVal == HIGH){
    digitalWrite(FAN, HIGH);
    digitalWrite(COOL, LOW);
    digitalWrite(HEAT, LOW);
  }
  else {
    digitalWrite(FAN, LOW);
    digitalWrite(COOL, LOW);
    digitalWrite(HEAT, LOW);
  }
  
  if (TempIn == LOW){
    setpoint = setpoint +1.0;
    display.print("Set: "); display.print(setpoint); display.println(" C");
    yield();
    display.display();
    delay(100);

  } else if (TempDe == LOW){
    setpoint = setpoint -1.0;
    display.print("Set: "); display.print(setpoint); display.println(" C");
    yield();
    display.display();
    delay(100);
  } else {

  }

  delay(1000);
  
}
