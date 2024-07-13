//Oxford Aerospace and Rocketry 2024
#include <SPI.h>
#include <LoRa.h>
#include <TinyGPSPlus.h>
#include "CRC16.h"
#include "CRC.h"

//Define device details
String FlightName = "OXHABTEST1";
String DEVICE = "Pico Tracker";
String VERSION = "V1.0";

// The TinyGPSPlus and Checksum objects
TinyGPSPlus gps;
CRC16 crc;

//Define the GPS Pins
UART GPSserial(4, 5, NC, NC);

//LoRa Transmission freqeuncy (hz) 
int freq = 433E6; 

//Setup a message counter
int count = 0;

//Setup the clock
char hms[18];

//Setup the altitude
char altitude[10];

void setup() {
  //Setup the serial connection
  Serial.begin(57600);
  Serial.println("");
  Serial.print(DEVICE);
  Serial.print("");
  Serial.println(VERSION);
  Serial.println("");

  //Initialise the GPS connection
  GPSserial.begin(9600);

  if (!LoRa.begin(freq)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //Limit power to the UK regulation 10 mW
  LoRa.setTxPower(10);
}

void loop() {
  while (GPSserial.available() > 0) {
    //Pass the raw GPS data to the tiny GPS library
    gps.encode(GPSserial.read());
    //Send a new message every time the GPS time updates
    if (gps.time.isUpdated()) {
      sendInfo();
    }
  }
}

void sendInfo() {
  //Create a UKHAS Standard message (using: https://www.daveakerman.com/?p=2987)
  char s[100];
  formatTime(gps.time.hour(), gps.time.minute(), gps.time.second());
  formatAltitude(gps.altitude.meters());

  /**************************
  *REMOVE +0.2 BEFORE FLIGHT*
  ***************************/
  
  snprintf(s, sizeof(s), "$$%s,%i,%s,%f,%f,%s", FlightName.c_str(), count, hms, gps.location.lat()+0.2, gps.location.lng()+0.2, altitude);
  Serial.print("Sending packet: ");
  Serial.println(count);

  LoRa.beginPacket();
  LoRa.print(s);

  //Append the CRC16 Checksum to the end of the message
  LoRa.print("*");
  LoRa.print(calcCRC16((uint8_t *)s, sizeof(s)), HEX);
  LoRa.endPacket();

  count++;
}

void formatTime(int h, int m, int s) {
  //Setup Arrays for Storing Time Strings
  char H[4];
  char M[4];
  char S[4];

  //Add leading zero
  if (h < 10) {
    snprintf(H, sizeof(H), "0%i", h);
  } else {
    snprintf(H, sizeof(H), "%i", h);
  }
  if (m < 10) {
    snprintf(M, sizeof(M), "0%i", m);
  } else {
    snprintf(M, sizeof(M), "%i", m);
  }
  if (s < 10) {
    snprintf(S, sizeof(S), "0%i", s);
  } else {
    snprintf(S, sizeof(S), "%i", s);
  }

  //Format the time
  snprintf(hms, sizeof(hms), "%s:%s:%s", H, M, S);
}

void formatAltitude(float alt) {
  int length = String(int(round(alt))).length();
  if (length > 5) {
    //Set maxiumum altitude
    snprintf(altitude, sizeof(altitude), "99999");
  } else {
    //Add leading zeros
    snprintf(altitude, sizeof(altitude), "%05d", int(round(alt)));
  }
}
