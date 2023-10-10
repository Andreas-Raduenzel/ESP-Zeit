#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <LedControl.h> // Für die Ansteuerung des MAX7219
#include "Fonts.h"

const char* ssid = "FritzRa";
const char* password = "07149590308496325159";

unsigned long previousMillis = 0;
const long interval = 1000;  // Interval to blink LED (in milliseconds)


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

    for(int i=0;i<4;i++){
    lc.shutdown(i,false);
    lc.setIntensity(i,5);
    lc.clearDisplay(i);
  }
  }

  timeClient.begin();
  timeClient.setTimeOffset(7200); 

  // Initialisiere das MAX7219 Display
 for(int i=0;i<4;i++){
    lc.shutdown(i,false);
    lc.setIntensity(i,1);
    lc.clearDisplay(i);
  }
}

void loop() {
  timeClient.update();

  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  Serial.println(timeClient.getFormattedTime());

  // Zeige die Zeit auf dem MAX7219 an
  //Digit 1,2,3 und 4

for(int x=0;x<8;x++) {

  lc.setColumn (0,1+x, zahl[minutes % 10] [0+x]);//Digit4  Einerstelle der Minute
  lc.setColumn (1,1+x, zahl[ minutes / 10] [0+x]); // Digit3 Zehnerstelle der Minute
  lc.setColumn (2,0+x, zahl[hours % 10] [0+x]); //Digit2 Einerstelle der Stunde
  //lc.setColumn (3,0+x, zahl[hours / 10] [0+x]);// Digit1 Zehnerstelle der Stunde
  if (hours / 10==0){
                                            lc.setColumn (3,0+x,zahl[10] [0+x]);//Digit1 alle LEDs aus
                                            }else{lc.setColumn (3,0+x, zahl[hours / 10] [0+x]);// Digit1
                                            }     
unsigned long currentMillis = millis();
 if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

  lc.setLed(2,2,7,true);// Doppelpunkt 1 an
  lc.setLed(2,5,7,true);// Doppelpunkt 2 an
  delay(200);
}else{
  lc.setLed(2,2,7,false);// Doppelpunkt 1 aus
  lc.setLed(2,5,7,false);// Doppelpunkt 2 aus 
}
  }
    
}


