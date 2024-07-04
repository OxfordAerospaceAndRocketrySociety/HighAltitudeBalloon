#Oxford Aerospace and Rocketry Society 2024
import serial.tools.list_ports
import tkinter as tk
import serialRead

if __name__ == "__main__":
    ser = serialRead.openPortGUI()
    while True:
        try:
            [name,count,time,lat,long,alt]=serialRead.decodeUKHASmessage(str(ser.readline()))
            print("Flight Name - %s\nMessage Count  - %s\nTime - %s\nLatitude  - %s\nLongitude - %s\nAltitude - %s\n"%(name,count,time,lat,long,alt))
        except KeyboardInterrupt:
            ser.close()