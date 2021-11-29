#include <TroykaMQ.h>
#include <TroykaButton.h>
#include <TroykaIMU.h>
#include <LiquidCrystalRus.h>
#include <Wire.h>
#include <TroykaDHT.h>
#include <TroykaRTC.h>
#define LEN_TIME       12
#define PIN_MQ9        A1
#define PIN_MQ9_HEATER 1
#define vppin          A0
#define zoomer_pin     12
RTC clock;
Barometer barometer;
TroykaButton bl(8);
TroykaButton br(6);
MQ9 mq9(PIN_MQ9, PIN_MQ9_HEATER);
char time[LEN_TIME];
int temp,hum,bar,VP,dist;
String timeStr,dateStr,weekDayStr;
int mode=0;
LiquidCrystalRus lcd( 11, 10, 5, 4, 3, 2);
DHT dht(9, DHT11);
void setup() {
  lcd.begin(20, 4);
  dht.begin();
  clock.begin();
  barometer.begin();
  bl.begin();
  br.begin();
  mq9.cycleHeat();
  pinMode(zoomer_pin, OUTPUT);
  //clock.set(__TIMESTAMP__);
  lcd.setCursor(7, 1);
  lcd.print("\x42\x4F\x52\x43\x48");
  lcd.setCursor(5, 2);
  lcd.print("НУК ВЕР. 2");
  tone(zoomer_pin, 400, 200);
  delay(200);
  tone(zoomer_pin, 600, 200);
  delay(200);
  tone(zoomer_pin, 800, 200);
  delay(2000);
  lcd.clear();
}

void loop() {
  ints();
  if (bl.isClick() && mode == 1){
    mode = 0;
    tone(zoomer_pin, 500, 100);
    lcd.clear();
  }if (br.isClick() && mode == 0){
    mode = 1;
    tone(zoomer_pin, 500, 100);
    lcd.clear();
  }
  
  if(mode == 0){
  times(0,0);
  temps(2,1);
  humi(12,1);
  bars(2,2);
  vp(0,3);
  }
  if (mode == 1){
    gass(0,0);
  }
}
void ints (){
  VP = analogRead(vppin);
  bar = barometer.readPressureMillimetersHg();
  temp = dht.getTemperatureC();
  hum = dht.getHumidity();
  clock.getTimeStamp(timeStr, dateStr, weekDayStr);
  clock.read();
  dht.read();
  bl.read();
  br.read();
}
void times (int cur, int cur1) {
  lcd.setCursor(cur,cur1);
  lcd.print(timeStr);
  lcd.setCursor(cur+10,cur1);
  lcd.print(dateStr);
}
void temps (int cur, int cur1){
  lcd.setCursor(cur,cur1);
  lcd.print(temp);
  lcd.print("\x99""C");
}
void humi (int cur, int cur1){
  lcd.setCursor(cur,cur1);
  lcd.print(hum);
  lcd.print("\x25");
}
void bars (int cur, int cur1) {
  lcd.setCursor(cur,cur1);
  lcd.print(bar);
  lcd.print("мм рт. ст.");
}
void gass (int cur, int cur1){
   if (!mq9.isCalibrated()) {
    lcd.setCursor(cur,cur1);
    lcd.print("калибровка...");
    lcd.setCursor(cur,cur1+1);
    lcd.print("подождите");
    delay(5000);
    mq9.calibrate();
    mq9.cycleHeat();
    lcd.clear();
   }
   if (mq9.isCalibrated()) {
    lcd.setCursor(cur,cur1);
    lcd.print("\x4c\x50\x47\x3a");
    lcd.print(mq9.readLPG());
    lcd.print(" \x70\x70\x6d");
    lcd.setCursor(cur,cur1+1);
    lcd.print("\x4D\x65\x74\x68\x3a");
    lcd.print(mq9.readMethane());
    lcd.print(" \x70\x70\x6d");
    lcd.setCursor(cur,cur1+2);
    lcd.print("\x43\x4d\x3a");
    lcd.print(mq9.readCarbonMonoxide());
    lcd.print(" \x70\x70\x6d");
    mq9.cycleHeat();
   }
}
void vp (int cur, int cur1){
  lcd.setCursor(cur,cur1);
  lcd.print("ВП:");
  lcd.print(VP);
  if (VP >= 0 && VP <= 300) {
    lcd.setCursor(cur+7,cur1);
    lcd.print("сухая почва");
  }
  else if (VP >= 300 && VP <= 600){
    lcd.setCursor(cur+7,cur1);
    lcd.print("влажная почва");
  }
  else if (VP >= 600 && VP <= 750){
    lcd.setCursor(cur+7,cur1);
    lcd.print("датчик в воде");
  }
}
