#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <ESP8266WebServer.h>
ESP8266WebServer webserver(80); 
LiquidCrystal_I2C lcd(0x27,16,2);

#define buzzer 13
#define led 2
#define PIR 12
#define FIREBASE_HOST "motion-detection-system-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "6EbAuvPTrUGSE8F5yP3lOUEg57BGgazb6Ln1l6v9"
int pirState;
String str;
String str1;
unsigned long lasTime = 0;
unsigned long lasTime_1 = 0;
const long interval = 5000;
const long interval_1 = 1000;
unsigned long currenTime;
unsigned long currenTime_1;

FirebaseData firebaseData;
FirebaseData loadData;
void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.init();
  lcd.backlight();
  pinMode(buzzer, OUTPUT);
  pinMode(PIR,INPUT);
  pinMode(led,OUTPUT);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin("8pxX2", "coldmineral**");
  while(WiFi.status() !=WL_CONNECTED){
    digitalWrite(led,HIGH);
    Serial.print(".");
    lcd.clear();
    lcd.print("Connecting to");
    lcd.setCursor(0,1);
    lcd.print("Mobile Hotspot");
    delay(1000);
}
  Serial.println();
  Serial.println("Connected");
  digitalWrite(led,LOW);
  lcd.clear();
  lcd.print("Connected to");
  lcd.setCursor(0,1);
  lcd.print("8pxX2");
  delay(1000);
}

void loop() {
  lcd.clear();
  lcdprint();
  str1= String("NO MOTION DETECTED");
  str = String("MOTION DETECTED");
  pirState = digitalRead(PIR);
  if (pirState==HIGH){
    currenTime_1 = millis();
    if (currenTime_1 - lasTime_1 >= interval_1){
      lasTime_1 = currenTime_1;
      digitalWrite(buzzer,HIGH);
      digitalWrite(led,HIGH);
      }
      else{
        digitalWrite(buzzer,LOW);
        digitalWrite(led,LOW);
        }
  }
  else{
    digitalWrite(buzzer,LOW);
    digitalWrite(led,LOW);
  }
  currenTime = millis();
 if(currenTime - lasTime >= interval){
    lasTime = currenTime;
    lcdprint();
 }
     
}
void lcdprint(){
  if(pirState==HIGH){
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("MOTION"); 
    lcd.setCursor(4,1);
    lcd.print("DETECTED");
    Firebase.setString(firebaseData,"Environment",str);
  }
  else{
    lcd.setCursor(3,0);
    lcd.print("NO MOTION"); 
    lcd.setCursor(4,1);
    lcd.print("DETECTED");
    Firebase.setString(firebaseData,"Environment",str1);
  }
}



