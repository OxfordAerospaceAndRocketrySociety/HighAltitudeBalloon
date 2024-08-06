# Setup
## Arduino Setup
1. Install the [arduino ide](https://www.arduino.cc/en/software), latest version (something like 2.3.x). Make sure you install any drivers when prompeted.
2. Open the board manager using (the circuit board icon).
3. Search for and install Arduino Mbed OS RP2040 Boards.
4. Download the [TinyGPS++](https://github.com/mikalhart/TinyGPSPlus/archive/master.zip) Library.
5. Open Sketch -> Include Library -> Add .ZIP Library and select the zip file you just downloaded.
6. Open the library manager ```Ctrl+Shift+I``` and install ```LoRa by Sandeep Mistry```.
7. Install ```CRC by Rob Tillaart``` in the library manager.
8. You're done!
## Python Dependencies
Please use pip to install `pyserial`, `crc`, `sondehub`and `tkintermapview`.
## KiCad Libraries
Pi Pico - https://github.com/ncarandini/KiCad-RP-Pico