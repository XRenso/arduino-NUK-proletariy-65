#include <TroykaMQ.h>
#include <TroykaButton.h>
#include <TroykaIMU.h>
#include <LiquidCrystalRus.h>
#include <Wire.h>
#include <TroykaDHT.h>
#include <TroykaRTC.h>

#define LEN_TIME 12
#define PIN_MQ9        A1
#define PIN_MQ9_HEATER 1
#define vppin          A0


char time[LEN_TIME];
int bar;
int temp;
int hum;
int hour;
int seconds;
int minute;
int mode = 0;
int max_mode = 1;
int VP;


RTC clock;
Barometer barometer;
LiquidCrystalRus lcd(12, 10, 11, 5, 4, 3, 2);
DHT dht(9, DHT11);
TroykaButton bl(8);
TroykaButton br(6);
MQ9 mq9(PIN_MQ9, PIN_MQ9_HEATER);



void setup() {
  lcd.begin(20, 4);
  dht.begin();
  clock.begin();
  clock.set(__TIMESTAMP__);
  startScreen();
  barometer.begin();
  bl.begin();
  br.begin();
   mq9.cycleHeat();
}


void Curs(int weight, int hight)
{
   lcd.setCursor(weight,hight);
  
}


void loop() {
   INIT(); 

   Screen_State(2);
   delay(29);
   changeScreenState();
  
   
   delay(29);
}



void Screen_State(int getORchange)
{
  if(getORchange == 1)
  {
    return mode;
  }
  else{
    if(bl.isClick() && mode == 0 || mode != 0 && mode != max_mode )
  {
    mode++;
    lcd.clear();
  }
  
  else if (bl.isClick() && mode == max_mode)
  {
    mode = 0;
    lcd.clear();
  }
  
  else if(br.isClick() && mode == max_mode || mode != 0 && mode != 0 )
  {
    mode--;
    lcd.clear();
  }
  
  else if (bl.isClick() && mode == 0)
  {
    mode = max_mode;
    lcd.clear();
  }
  }
}




void changeScreenState()
{
  switch(mode){
    case 0:
       Time(1); 
       
       Curs(3,1);
       
       Temperature();
       
       Curs(13,1);
       
       Hum();
       
       Curs(1,2);
       
       Bar();

       vp(0,3);
       
    case 1:
        gas(0,0);
        delay(29);      
      
  }
}



void INIT ()
{
  VP = analogRead(vppin);
  delay(500);
  bar = barometer.readPressureMillimetersHg();
  delay(500);
  temp = dht.getTemperatureC();
   hum = dht.getHumidity();
  delay(500);
  hour = clock.getHour();
  minute = clock.getMinute();
  seconds = clock.getSecond();
  clock.read();
  dht.read();
  bl.read();
  br.read();
}


void Temperature()
{
  delay(500);
  lcd.print(temp);
  lcd.print("\x99""C"); 
}

void Bar () {
  delay(500);
  lcd.print(bar);
  lcd.print("мм рт. ст.");
  
}




void Time(int isChangeCurs)
{
  delay(500);
  if (isChangeCurs == 1)
   { Curs(6,0); }
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(seconds);
}



void Hum ()
{ 
  lcd.print(hum);
  lcd.print("\x25");
  delay(500);
}

void startScreen()
{
  Curs(7, 1);
  lcd.print("\x42\x4F\x52\x43\x48");
  Curs(5, 2);
  lcd.print("НУК ВЕР. 1");
  delay(2000);
  lcd.clear();
}



void gas (int cur, int cur1){
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
    delay(100);
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
    lcd.clear();
  }
  else if (VP >= 300 && VP <= 600){
    lcd.setCursor(cur+7,cur1);
    lcd.print("влажная почва");
    lcd.clear();
  }
  else if (VP >= 600 && VP <= 750){
    lcd.setCursor(cur+7,cur1);
    lcd.print("датчик в воде");
    lcd.clear();
  }
}
