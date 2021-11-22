#include <TroykaIMU.h>
#include <LiquidCrystalRus.h>
#include <Wire.h>
#include <TroykaDHT.h>
#include <TroykaRTC.h>
#define LEN_TIME 12
RTC clock;
Barometer barometer;
char time[LEN_TIME];
LiquidCrystalRus lcd(12, 10, 11, 5, 4, 3, 2);
DHT dht(9, DHT11);
void setup() {
  lcd.begin(20, 4);
  dht.begin();
  clock.begin();
  barometer.begin();
  clock.set(__TIMESTAMP__);
  lcd.setCursor(7, 1);
  lcd.print("\x42\x4F\x52\x43\x48");
  lcd.setCursor(5, 2);
  lcd.print("НУК ВЕР. 1");
  delay(2000);
  lcd.clear();
}

void loop() {
  int bar = barometer.readPressureMillimetersHg();
  int temp = dht.getTemperatureC();
  int hum = dht.getHumidity();
  int hour = clock.getHour();
  int minute = clock.getMinute();
  int seconds = clock.getSecond();
  clock.read();
  dht.read();
  lcd.setCursor(6,0);
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(seconds);
  lcd.setCursor(3,1);
  lcd.print(temp);
  lcd.print("\x99""C");
  lcd.setCursor(13,1);
  lcd.print(hum);
  lcd.print("\x25");
  lcd.setCursor(1,2);
  lcd.print(bar);
  lcd.print("мм рт. ст.");
}
