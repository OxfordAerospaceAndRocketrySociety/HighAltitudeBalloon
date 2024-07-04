#Oxford Aerospace and Rocketry Society 2024
import serial.tools.list_ports
import tkinter as tk

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
    #'$$FLIGHTNAME,COUNT,TIME,LAT,LONG,ALTITUDE'
    parts = msg.split(",")
    name = parts[0][12:]
    [count,time,lat,long] = parts[1:5]
    alt = parts[5][0:5]
    return([name,count,time,lat,long,alt])

if __name__ == "__main__":
    ser = openPortTerm()
    while True:
        try:
            [name,count,time,lat,long,alt]=decodeUKHASmessage(str(ser.readline()))
            print("Flight Name - %s\nMessage Count  - %s\nTime - %s\nLatitude  - %s\nLongitude - %s\nAltitude - %s\n"%(name,count,time,lat,long,alt))
        except KeyboardInterrupt:
            ser.close()
    