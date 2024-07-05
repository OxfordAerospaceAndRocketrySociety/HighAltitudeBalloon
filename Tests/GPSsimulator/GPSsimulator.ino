//Oxford Aerospace and Rocketry 2024
//A test of the GPS Module and UKHAS message creation
#include <TinyGPSPlus.h>
#include "CRC16.h"
#include "CRC.h"

//Define device details
String FlightName = "OXHABTEST0";
String DEVICE = "Pico Tracker";
String VERSION = "GPS Test 2";

//Checksum object
CRC16 crc;

//Setup a message counter
int count = 0;

//Setup the clock
String hms = "12:34:56";

//Setup the altitude
String altitude = "00123";

//Setup latitude and longitude
float lat = 51.75872785;
float lng = -1.26279873;

void setup() {
  //Setup to work with HAB Gateway etc
  Serial.begin(57600);
  Serial.println("");
  Serial.print(DEVICE);
  Serial.print("");
  Serial.println(VERSION);
  Serial.println("");
}

void loop() {
      displayInfo();
      delay(1000);
}

void displayInfo() {
  //Create a UKHAS Standard message (using: https://www.daveakerman.com/?p=2987)
  char s[100];
  float latRnd = float(random(-250,250))/1000+lat;
  float lngRnd = float(random(-250,250))/1000+lng;
  snprintf(s, sizeof(s), "$$%s,%i,%s,%f,%f,%s", FlightName.c_str(), count, hms.c_str(), latRnd, lngRnd, altitude.c_str());
  Serial.print("Message=");
  Serial.print(s);

  //Append the CRC16 Checksum to the end of the message
  Serial.print("*");
  Serial.println(calcCRC16((uint8_t *)s, sizeof(s)), HEX);

  count++;
}