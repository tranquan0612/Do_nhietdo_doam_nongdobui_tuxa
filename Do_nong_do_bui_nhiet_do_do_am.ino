#include <DHT.h>
#include <SharpGP2Y10.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL63aSjbWIb"
#define BLYNK_TEMPLATE_NAME "Do nong do bui nhiet do do am"  
#define REPORTING_PERIOD_MS     1000
//#define XANH  D7
//#define VANG  D6
//#define DO  D5
LiquidCrystal_I2C lcd(0x27, 16, 2);

float humidity;
float temperature;
float dustDensity;

const int voPin = A0;
const int ledPin = 0;

SharpGP2Y10 dustSensor(voPin, ledPin);

const int DHTPIN = 13; 
const int DHTTYPE = DHT22; 
DHT dht(DHTPIN, DHTTYPE);

char auth[] = "3cjJLSXq6I4r0EKF3dMq5W1XsqHtNA-A";             // Authentication Token Sent by Blynk
char ssid[] = "OPPO A5 2020";        //WiFi SSID
char pass[] = "duynhat01";        //WiFi Password
uint32_t tsLastReport = 0;
void setup() {
  dht.begin();
  Serial.begin(115200);
  Serial.print("Dang tinh toan");
  Blynk.begin(auth, ssid, pass);
  pinMode(D6,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(D4,OUTPUT);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Dang quet...");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  dustDensity = dustSensor.getDustDensity();
  // Kiểm tra xem cảm biến đã đọc được dữ liệu chưa
  if (isnan(humidity) || isnan(temperature) || isnan(dustDensity)) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("Can't find");
    lcd.setCursor(0,1);
    lcd.println("sensor");
    delay(1000);
    return;
  }
  Blynk.run();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
  // Hiển thị dữ liệu nhiệt độ và độ ẩm lên Serial Monitor
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temperature,1);
  lcd.print("C,H:");
  lcd.print(humidity,1);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("PM2.5:");
  lcd.print(dustDensity,1);
  lcd.print("mg/m3");
  if(dustDensity<=0.3 && 0<=dustDensity){
    digitalWrite(D6,HIGH);
    digitalWrite(D5,LOW);
    digitalWrite(D4,LOW);
  }
  if(dustDensity<=0.6 && dustDensity>0.3){
    digitalWrite(D6,LOW);
    digitalWrite(D5,HIGH);
    digitalWrite(D4,LOW);
  }
  if(dustDensity<=1 && dustDensity>0.6){
    digitalWrite(D6,LOW);
    digitalWrite(D5,LOW);
    digitalWrite(D4,HIGH);
  }
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, dustDensity);
  tsLastReport = millis();
  }
}
