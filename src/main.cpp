#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "LedControl.h"

const char* ssid = "FritzRa";
const char* password = "07149590308496325159";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Verbindung zum WLAN herstellen...");
  }

  timeClient.begin();
  timeClient.setTimeOffset(7200);  // Anpassen Sie die Zeitzone nach Bedarf
}

void loop() {
  timeClient.update();
  timeClient.getHours();
  

  Serial.println(timeClient.getFormattedTime());


  delay(1000);  // Warten Sie 1 Sekunde, bevor Sie die Uhrzeit erneut aktualisieren
}
