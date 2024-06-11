//Code from: https://randomnerdtutorials.com/guide-to-neo-6m-gps-module-with-arduino/
// The serial connection to the GPS module
UART ss(4, 5, NC, NC);

char setNav[] = {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xDC};
int count = 0;

void SetupGPSCommand()
{
  for (int i = 0; i < sizeof(setNav); i++)
  {
    ss.write(setNav[i]);
  }
}

void setup(){
  Serial.begin(9600);
  ss.begin(9600);

  delay(3000);
  Serial.println("Sending GPS setup command");
  SetupGPSCommand();
}

void loop(){

    while (ss.available() > 0){
      // get the byte data from the GPS
      byte gpsData = ss.read();
      Serial.write(gpsData);

      if (gpsData == '\n')
      {
        count++;
      }

      if (count == 5)
      {
        count = 0;
        SetupGPSCommand();
        Serial.println("GPS SETUP SENT****************************************************");
      }
  }
}