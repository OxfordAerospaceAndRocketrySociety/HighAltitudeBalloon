//Code from: https://randomnerdtutorials.com/guide-to-neo-6m-gps-module-with-arduino/
// The serial connection to the GPS module
UART ss(4, 5, NC, NC);

void setup(){
  Serial.begin(9600);
  ss.begin(9600);
}

void loop(){
  while (ss.available() > 0){
    // get the byte data from the GPS
    byte gpsData = ss.read();
    Serial.write(gpsData);
  }
}