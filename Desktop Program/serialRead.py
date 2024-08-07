#Oxford Aerospace and Rocketry Society 2024
import serial.tools.list_ports
import tkinter as tk
from crc import Calculator, Configuration

def getDev():
    #Find the ports and filter out the unused ones, terminal input
    devices = [comport.device for comport in serial.tools.list_ports.comports()]
    filteredDevices = []
    for device in devices:
        if len(device) < 9:
            filteredDevices.append(device)
        else:
            if device[8] != "S":
                filteredDevices.append(device)
    return filteredDevices

def openPortTerm():
    devices = getDev()
    #Allow the user to choose a port
    print("AVAILABLE PORTS:")
    for i in range(len(devices)):
        print("%i - %s"%(i,devices[i]))
    portNo = int(input("Please enter the port number: "))
    ser = serial.Serial(devices[portNo])
    return ser

def openPortGUI():
    #Setup window
    global window
    window = tk.Tk()
    window.title("Port Selection")
    #Give title to window
    title = tk.Label(text="Available Ports")
    title.pack()
    #Select devices
    global device
    devices = getDev()
    device = tk.StringVar(window)
    device.set(devices[0]) # default value
    #Setup the serial device selector
    dropdown = tk.OptionMenu(window, device, *devices)
    dropdown.pack()
    #Setup the button for selecting devices
    button = tk.Button(window, text = "Select", command = selectPort)
    button.pack()
    #Run window
    window.mainloop()
    return ser

def selectPort():
    #Command for retrieving selection and destroying window
    global ser
    ser = serial.Serial(device.get())
    window.destroy()

def decodeUKHASmessage(msg):
    #Configure the CRC checksum calculator
    config = Configuration(
        width=16,
        polynomial=0x1021,
        init_value=0xFFFF,
        final_xor_value=0x0000,
        reverse_input=False,
        reverse_output=False,
    )
    calculator = Calculator(config) 
    #Get rid of any extra useless parts of serial input
    msg = msg[2:-5]
    #'$$FLIGHTNAME,COUNT,TIME,LAT,LONG,ALTITUDE'
    if msg[0:10] == "Message=$$" and "*" in msg:
        #Split out CRC and message
        [raw,crc] = msg.split("*")
        #Data which the checksum is calculated over
        data = raw[10:]
        #Recieved CRC
        try:
            crcInt = int(crc, 16)
        #4 CRC Doesn't Match (CRC Corrupted)
        except ValueError:
            return([4,msg])
        #Calculate CRC from data
        calcCrc = calculator.checksum(str.encode(data))
        #0 CRC matches
        if calcCrc == crcInt:
            parts = msg.split(",")
            name = parts[0][10:]
            [count,time,lat,long] = parts[1:5]
            alt = parts[5].split("*")[0]
            return([0,name,count,time,lat,long,alt,crc])
        #2 CRC doesn't match
        else:
            return([2,msg])
    #1 Recieved RSSI
    elif msg[0:4] == "RSSI":
        RSSI = msg[5:]
        return([1,RSSI])
    #3 Other error
    else:
        return([3,msg])

if __name__ == "__main__":
    ser = openPortTerm()
    while True:
        try:
            msg = str(ser.readline())
            decodedMsg=decodeUKHASmessage(msg)
            #Print data from message
            if decodedMsg[0] == 0:
                [name,count,time,lat,long,alt,crc] = decodedMsg[1:]
                print("Flight Name - %s\nMessage Count  - %s\nTime - %s\nLatitude  - %s\nLongitude - %s\nAltitude - %s\nCRC - %s"%(name,count,time,lat,long,alt,crc))
            #Print RSSI
            elif decodedMsg[0] == 1:
                RSSI = decodedMsg[1]
                print("RSSI - %s\n"%RSSI)
        except KeyboardInterrupt:
            ser.close()
    