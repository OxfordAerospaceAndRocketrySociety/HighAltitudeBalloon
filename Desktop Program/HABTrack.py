#Oxford Aerospace and Rocketry Society 2024
import tkinter as tk
import tkintermapview
import serialRead
import logging
from sondehub.amateur import Uploader

def update():
    decoded=serialRead.decodeUKHASmessage(str(ser.readline())) #[name,count,time,lat,long,alt,crc]
    #Decoded UKHAS Message
    if decoded[0] == 0:
        values = decoded[1:]
        for i in range(len(values)):
            labels[i]['text'] = "%s%s" % (fieldNames[i],values[i])
        marker.set_position(float(values[3]),float(values[4]))
        map_widget.set_position(float(values[3]),float(values[4]))
        #If upload to sondehub checked
        if uploadData.get() == True:
            uploader.add_telemetry(
            values[0], #Callsign
            values[2], #Time
            float(values[3]), #Latitude
            float(values[4]), #Longitude
            int(values[5]), #Altitude,
            int(values[1]) #Packet count
        )
    #Decoded RSSI
    elif decoded[0] == 1:
        RSSI = decoded[1]
        RSSILabel['text'] = "RSSI: %s" % RSSI
    else:
        print(decoded)
    window.after(100, update) # update again after 1000 ms

#Close the uploader, serial port and window
def closeSafely():
    uploader.close()
    ser.close()
    window.destroy()

if __name__ == "__main__":
    #Open selected serial port
    ser = serialRead.openPortGUI()
    values = ["N/A"]*7
    RSSI = ["N/A"]
    #Setup tkinter window
    window = tk.Tk()
    window.title("HAB Tracker")
    window.protocol("WM_DELETE_WINDOW", closeSafely)
    #Create labels
    fieldNames = ["Flight Name: ","Message Count: ","GPS Time: ","Latitude: ", "Longitude: ", "Altitude(m): ", "CRC: "]
    labelFrame = tk.Frame()
    labels = []
    for i in range(len(values)):
        labels.append(tk.Label(labelFrame, text = "%s%s" % (fieldNames[i],values[i])))
        labels[i].pack()
    RSSILabel = tk.Label(labelFrame, text = "RSSI: %s" % RSSI)
    RSSILabel.pack()
    #Create checkbox
    uploadData = tk.BooleanVar()
    uploadButton = tk.Checkbutton(labelFrame,text = "Upload to Sondehub",variable=uploadData)
    uploadButton.pack()
    #Create map widget
    map_widget = tkintermapview.TkinterMapView(window, width=500, height=500, corner_radius=0)
    labelFrame.grid(row=0, column=0, sticky="nsew")
    map_widget.grid(row=0,column=1, sticky="nsew")
    #Set position
    map_widget.set_position(51.75872785,-1.26279873)
    map_widget.set_zoom(10)
    #Create a marker
    marker = map_widget.set_marker(51.75872785,-1.26279873, text="Location")
    #Start uploader (CHANGE NAME)
    logging.basicConfig(format="%(asctime)s %(levelname)s:%(message)s", level=logging.INFO) #Logging output for uploader
    uploader = Uploader(
    #"MYCALL" for testing
    "MYCALL",
    #False for upload
    developer_mode=True)
    update()
    window.mainloop()