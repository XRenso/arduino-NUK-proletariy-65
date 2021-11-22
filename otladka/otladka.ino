#include <LiquidCrystalRus.h>
#include <Wire.h>
#include <TroykaDHT.h>
#include <TroykaRTC.h>
#define LEN_TIME 12
RTC clock;
char time[LEN_TIME];
LiquidCrystalRus lcd(12, 10, 11, 5, 4, 3, 2);
DHT dht(9, DHT11);
void setup() {
  lcd.begin(20, 4);
  dht.begin();
  clock.begin();
  clock.set(__TIMESTAMP__);
  startScreen();
}



void loop() {
  Time();
  lcd.setCursor(1,2);
  Temperature();
}


void Temperature()
{
  dht.read();
  lcd.print(dht.getTemperatureC());
  lcd.print("\x99""C");
}



void Time()
{
  clock.read();
  lcd.setCursor(6,0);
  lcd.print(clock.getHour());
  lcd.print(":");
  lcd.print(clock.getMinute());
  lcd.print(":");
  lcd.print(clock.getSecond());
}

void startScreen()
{
  lcd.setCursor(7, 1);
  lcd.print("\x42\x4F\x52\x43\x48");
  lcd.setCursor(5, 2);
  lcd.print("НУК ВЕР. 1");
  delay(2000);
  lcd.clear();
}
