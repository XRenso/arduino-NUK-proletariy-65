#include <TroykaMQ.h>
#include <TroykaButton.h>
#include <NewPing.h>
#include <TroykaIMU.h>
#include <LiquidCrystalRus.h>
#include <Wire.h>
#include <TroykaDHT.h>
#include <TroykaRTC.h>
#define LEN_TIME       12
#define TRIGGER_PIN    12
#define ECHO_PIN       13
#define MAX_DISTANCE   500
#define PIN_MQ9        A1
#define PIN_MQ9_HEATER 1
#define vppin          A0
RTC clock;
Barometer barometer;
TroykaButton bl(8);
TroykaButton br(6);
MQ9 mq9(PIN_MQ9, PIN_MQ9_HEATER);
char time[LEN_TIME];
int temp,hum,bar,lpg,meth,CM,VP,dist;
String timeStr,dateStr,weekDayStr;
int mode=0;
LiquidCrystalRus lcd( 11, 10, 5, 4, 3, 2);
DHT dht(9, DHT11);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
void setup() {
  lcd.begin(20, 4);
  dht.begin();
  clock.begin();
  barometer.begin();
  bl.begin();
  br.begin();
  mq9.cycleHeat();
  clock.set(__TIMESTAMP__);
  lcd.setCursor(7, 1);
  lcd.print("\x42\x4F\x52\x43\x48");
  lcd.setCursor(4, 2);
  lcd.print("НУК ВЕР. 1,5");
  delay(2000);
  lcd.clear();
}

void loop() {
  ints();
  if (bl.isClick() && mode == 1){
    mode = 0;
    lcd.clear();
  }if (br.isClick() && mode == 0){
    mode = 1;
    lcd.clear();
  }
  
  if(mode == 0){
  times(0,0);
  temps(2,1);
  humi(2,2);
  bars(12,1);
  dista(12,2);
  vp(1,3);
  }
  if (mode == 1){
    LPG(0,0);
    Cm(0,1);
    Meth(0,2);
    delay(29);
  }
  mq9.cycleHeat();
}
void ints (){
  VP = analogRead(vppin);
  CM = mq9.readCarbonMonoxide();
  meth = mq9.readMethane();
  lpg = mq9.readLPG();
  dist = sonar.ping_cm();
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
  lcd.print("мм");
}
void dista (int cur, int cur1) {
  lcd.setCursor(cur,cur1);
  lcd.print(dist);
  lcd.setCursor(cur + 2,cur1);
  lcd.print("см");
}
void LPG (int cur, int cur1){
  
  lcd.setCursor(cur, cur1);
  lcd.print("\x4c\x50\x47\x3A");
  if (mq9.atHeatCycleEnd()) {
  lcd.print(lpg);
  lcd.print("\x50\x50\x4d");
  }else{
    lcd.print(" нагрев, погоди");
  }
}void Cm (int cur, int cur1){
  
  lcd.setCursor(cur, cur1);
  lcd.print("\x43\x4d\x3a");
  if (mq9.atHeatCycleEnd()) {
  lcd.print(CM);
  lcd.print("\x50\x50\x4d");
  }else{
    lcd.print(" нагрев, погоди");
  }
}
void Meth (int cur, int cur1){
  
  lcd.setCursor(cur, cur1);
  lcd.print("\x4d\x65\x74\x68\x3A");
  if (mq9.atHeatCycleEnd()) {
  lcd.print(meth);
  lcd.print("\x50\x50\x4d");
  }else{
    lcd.print(" нагрев, погоди");
  }
}
void vp (int cur, int cur1){
  lcd.setCursor(cur,cur1);
  lcd.print("ВП: ");
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
