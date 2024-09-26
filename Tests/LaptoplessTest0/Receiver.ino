//Derived from the examples located at:
//https://github.com/sandeepmistry/arduino-LoRa/tree/master?tab=readme-ov-file

//Modified to signal reception of lora packets by changing the state of the board LED
//Perhaps I should include a check to verify the name given in lore packet eg OXHAB

#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  //while (!Serial);

  Serial.println("LoRa Receiver");
  pinMode(LED_BUILTIN, OUTPUT);

  if (!LoRa.begin(434.145E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

bool light = false;

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Message=");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.println("");
    Serial.print("RSSI:");
    Serial.println(LoRa.packetRssi());

    light = !light;
    
    digitalWrite(LED_BUILTIN, (light && HIGH) || LOW);
    
  }
}
