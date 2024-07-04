//Derived from the examples located at:
//https://github.com/sandeepmistry/arduino-LoRa/tree/master?tab=readme-ov-file
#include <SPI.h>
#include <LoRa.h>
#include <TinyGPSPlus.h>
#include "CRC16.h"
#include "CRC.h"

//Define device details
String FlightName = "OXHABTEST0";
String DEVICE = "Pico Tracker";
String VERSION = "LoRa Test 1";

// A sample NMEA stream.
const char *gpsStream =
  "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
  "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
  "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
  "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
  "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
  "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";

// The TinyGPSPlus object
TinyGPSPlus gps;

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
  delay(3000);
  Serial.begin(57600);
  Serial.println("");
  Serial.print(DEVICE);
  Serial.print("");
  Serial.println(VERSION);
  Serial.println("");

  if (!LoRa.begin(freq)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //Limit power to the UK regulation 10 mW
  LoRa.setTxPower(10);
}

void loop() {
  while (*gpsStream)
    if (gps.encode(*gpsStream++))
      sendInfo();
      delay(1000);
}

void sendInfo() {
  //Create a UKHAS Standard message (using: https://www.daveakerman.com/?p=2987)
  char s[100];
  formatTime(gps.time.hour(), gps.time.minute(), gps.time.second());
  formatAltitude(gps.altitude.meters());
  snprintf(s, sizeof(s), "$$%s,%i,%s,%f,%f,%s", FlightName.c_str(), count, hms, gps.location.lat(), gps.location.lng(), altitude);
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
