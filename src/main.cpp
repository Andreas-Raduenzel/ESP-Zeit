#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <LedControl.h> // Für die Ansteuerung des MAX7219

const char* ssid = "FritzRa";
const char* password = "07149590308496325159";

unsigned long previousMillis = 0;
const long interval = 1000;  // Interval to blink LED (in milliseconds)


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Die Parameter sind (DIN, CLK, CS, Anzahl der Displays)
LedControl lc=LedControl(15,12,13,4); //D8=15, D6=12, D7=13

int tc;
unsigned char zahl [11][8] =  
                {     {B00111100,B01100110,B01100110,B01100110,B01100110,B01100110,B01100110,B00111100},//0
                      {B00011000,B00111000,B00011000,B00011000,B00011000,B00011000,B00011000,B00111100},//1
                      {B00111100,B01100110,B00000110,B00001100,B00011000,B00110000,B01100000,B01111110},//2
                      {B00111100,B01100110,B00000110,B00011100,B00000110,B00000110,B01100110,B00111100},//3
                      {B00001110,B00011110,B00110110,B01100110,B01100110,B01111110,B00000110,B00000110},//4
                      {B01111110,B01100000,B01100000,B01111100,B00000110,B00000110,B01100110,B00111100},//5
                      {B00111100,B01100110,B01100000,B01111100,B01100110,B01100110,B01100110,B00111100},//6
                      {B01111110,B00000110,B00000110,B00001100,B00011000,B00011000,B00011000,B00011000},//7
                      {B00111100,B01100110,B01100110,B00111100,B01100110,B01100110,B01100110,B00111100},//8
                      {B00111100,B01100110,B01100110,B01100110,B00111110,B00000110,B01100110,B00111100},//9
                      {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000},//alle LEDs aus
                };

unsigned char zahlD2_Doppelpunkt_an [11][8] =  
                {     {B00111100,B01100110,B01100111,B01100110,B01100110,B01100111,B01100110,B00111100},//0
                      {B00011000,B00111000,B00011001,B00011000,B00011000,B00011001,B00011000,B00111100},//1
                      {B00111100,B01100110,B00000111,B00001100,B00011000,B00110001,B01100000,B01111110},//2
                      {B00111100,B01100110,B00000111,B00011100,B00000110,B00000111,B01100110,B00111100},//3
                      {B00001110,B00011110,B00110111,B01100110,B01100110,B01111111,B00000110,B00000110},//4
                      {B01111110,B01100000,B01100001,B01111100,B00000110,B00000111,B01100110,B00111100},//5
                      {B00111100,B01100110,B01100001,B01111100,B01100110,B01100111,B01100110,B00111100},//6
                      {B01111110,B00000110,B00000111,B00001100,B00011000,B00011001,B00011000,B00011000},//7
                      {B00111100,B01100110,B01100111,B00111100,B01100110,B01100111,B01100110,B00111100},//8
                      {B00111100,B01100110,B01100111,B01100110,B00111110,B00000111,B01100110,B00111100},//9
                      {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000},//alle LEDs aus
                };   

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Verbindung zum WLAN herstellen...");

    for(int i=0;i<4;i++){
    lc.shutdown(i,false);
    lc.setIntensity(i,1);
    lc.clearDisplay(i);
  }
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
  //Digit 1,2,3 und 4

for(int x=0;x<8;x++) {



  //lc.setRow(0, 0, hours / 10, false); // Erste Ziffer der Stunden
  //lc.setRow(0, 1, hours % 10, true);  // Zweite Ziffer der Stunden (mit Dezimalpunkt als Trenner)
  //lc.setRow(0, 2, minutes / 10, false); // Erste Ziffer der Minuten
  //lc.setRow(0, 3, minutes % 10, false); // Zweite Ziffer der Minuten
  lc.setRow (0,0+x,zahl[minutes % 10] [0+x]);//Digit4  Einerstelle der Minute
  lc.setRow (1,0+x, zahl[ minutes / 10] [0+x]); // Digit3 Zehnerstelle der Minute
  lc.setRow (2,0+x, zahl[hours % 10] [0+x]); //Digit2 Einerstelle der Stunde
  lc.setRow (3,0+x, zahl[hours / 10] [0+x]);// Digit1 Zehnerstelle der Stunde

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


