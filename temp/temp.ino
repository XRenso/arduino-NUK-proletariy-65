#include <QuadDisplay2.h>
#include <Wire.h>
#include <TroykaDHT.h>
#include <TroykaRTC.h>
#define LEN_TIME 12
RTC clock;
char time[LEN_TIME];
QuadDisplay qd(8);
DHT dht(9, DHT11);
void setup() {
  qd.begin();
  dht.begin();
  clock.begin();
  clock.set(__TIMESTAMP__);
}

void loop() {
  clock.read();
  dht.read();
  qd.displayTemperatureC(dht.getTemperatureC());
  delay(5000);
  qd.displayHumidity(dht.getHumidity());
  delay(5000);
  qd.displayScore(clock.getHour(), clock.getMinute(), true);
  delay(5000);
}
