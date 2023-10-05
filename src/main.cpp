#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <LedControl.h> // Für die Ansteuerung des MAX7219

const char* ssid = "FritzRa";
const char* password = "07149590308496325159";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Die Parameter sind (DIN, CLK, CS, Anzahl der Displays)
LedControl lc=LedControl(15,12,13,4); //D8=15, D6=12, D7=13

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Verbindung zum WLAN herstellen...");
  }

  timeClient.begin();
  timeClient.setTimeOffset(7200); 

  // Initialisiere das MAX7219 Display
  lc.shutdown(0,false);  // Schalte den Energiesparmodus aus
  lc.setIntensity(0,8);  // Setze die Helligkeit (0 min, 15 max)
  lc.clearDisplay(0);    // Lösche das Display
}

void loop() {
  timeClient.update();

  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  Serial.println(timeClient.getFormattedTime());

  // Zeige die Zeit auf dem MAX7219 an
  lc.setDigit(0, 0, hours / 10, false); // Erste Ziffer der Stunden
  lc.setDigit(0, 1, hours % 10, true);  // Zweite Ziffer der Stunden (mit Dezimalpunkt als Trenner)
  lc.setDigit(0, 2, minutes / 10, false); // Erste Ziffer der Minuten
  lc.setDigit(0, 3, minutes % 10, false); // Zweite Ziffer der Minuten
  
  delay(1000);
}
