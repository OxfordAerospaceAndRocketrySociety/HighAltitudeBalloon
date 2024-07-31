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

//Setup message counters
int count = 0;
int pkt_count = 0;

//Setup the clock
String hms;

//Setup the altitude
String altitude;

void setup() {
  //Setup the serial connection
  Serial.begin(57600);
  Serial.println("");
  Serial.print(DEVICE);
  Serial.print("");
  Serial.println(VERSION);
  Serial.println("");

  //Setup the builtin LED
  pinMode(LED_BUILTIN, OUTPUT);

  //Initialise the GPS connection
  GPSserial.begin(9600);

  if (!LoRa.begin(freq)) {
    Serial.println("Starting LoRa failed!");
    //Flash error code
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
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
  String message;
  formatTime(gps.time.hour(), gps.time.minute(), gps.time.second());
  formatAltitude(gps.altitude.meters());

  message = FlightName+","+String(count)+","+hms+","+String(gps.location.lat(),6)+","+String(gps.location.lng(),6)+","+String(altitude);

  Serial.print("Sending packet: ");
  Serial.println(pkt_count);

  if (gps.location.lat() != 0.0) {
    digitalWrite(LED_BUILTIN, LOW);
    LoRa.beginPacket();
    LoRa.print("$$");
    LoRa.print(message);

    //Append the CRC16 Checksum to the end of the message
    LoRa.print("*");
    //CRC16 Configuration - Polynome: 0x1021, Initial Value: 0xFFFF, Final Value: 0x0000
    LoRa.print(calcCRC16((uint8_t *)message.c_str(),message.length(),4129,65535,0), HEX);
    LoRa.endPacket();
    count++;
  }
  else {
    digitalWrite(LED_BUILTIN, HIGH);
    LoRa.beginPacket();
    LoRa.print("??");
    LoRa.print(FlightName);
    LoRa.print(" NO GPS FIX - TIME: ");
    LoRa.print(hms);
    LoRa.endPacket();
  }
  pkt_count++;
}

void formatTime(int h, int m, int s) {
  //Setup Arrays for Storing Time Strings
  String H;
  String M;
  String S;

  //Add leading zero
  if (h < 10) {
    H = "0" + String(h);
  } else {
    H = String(h);
  }
  if (m < 10) {
    M = "0" + String(m);
  } else {
    M = String(m);
  }
  if (s < 10) {
    S = "0" + String(s);
  } else {
    S = String(s);
  }

  //Format the time
  hms = H+":"+M+":"+S;
}

void formatAltitude(float alt) {
  int length = String(int(round(alt))).length();
  if (length > 5) {
    //Set maxiumum altitude
    altitude = "99999";
  } else {
    altitude = String(int(round(alt)));
  }
}
