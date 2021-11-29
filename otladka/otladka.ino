#include <TroykaMQ.h>
#include <TroykaButton.h>
#include <NewPing.h>
#include <TroykaIMU.h>
#include <LiquidCrystalRus.h>
#include <Wire.h>
#include <TroykaDHT.h>
#include <TroykaRTC.h>

#define LEN_TIME 12
#define TRIGGER_PIN  12
#define ECHO_PIN     13
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
int dist;
int mode=0;
int max_mode = 1;



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



   
   Time(1); 
   
   Curs(3,1);
   
   Temperature();
   
   Curs(13,1);
   
   Hum();
   
   
   Curs(1,2);
   
   
   Bar();
   
   Curs(1,3);
   
   Dist();
   delay(29);
}

void INIT ()
{
  delay(500);
  dist = sonar.ping_cm();
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
void Dist () {
  delay(500);
  lcd.print(dist);
  lcd.print("см");
}
