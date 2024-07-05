#Oxford Aerospace and Rocketry Society 2024
import tkinter as tk
import tkintermapview
import serialRead

def update():
    values=serialRead.decodeUKHASmessage(str(ser.readline())) #[name,count,time,lat,long,alt]
    for i in range(len(values)):
        labels[i]['text'] = "%s%s" % (fieldNames[i],values[i])
    marker.set_position(float(values[3]),float(values[4]))
    window.after(1000, update) # update again after 1000 ms

if __name__ == "__main__":
    #Open selected serial port
    ser = serialRead.openPortGUI()
    values=serialRead.decodeUKHASmessage(str(ser.readline())) #[name,count,time,lat,long,alt]
    #Setup tkinter window
    window = tk.Tk()
    window.title("HAB Tracker")
    #Create labels
    fieldNames = ["Flight Name: ","Message Count: ","GPS Time: ","Latitude: ", "Longitude: ", "Altitude(m): "]
    labelFrame = tk.Frame()
    labels = []
    for i in range(len(values)):
        labels.append(tk.Label(labelFrame, text = "%s%s" % (fieldNames[i],values[i])))
        labels[i].pack()
    # create map widget
    map_widget = tkintermapview.TkinterMapView(window, width=500, height=500, corner_radius=0)
    labelFrame.grid(row=0, column=0, sticky="nsew")
    map_widget.grid(row=0,column=1, sticky="nsew")
    # set position
    map_widget.set_position(51.75872785,-1.26279873)
    map_widget.set_zoom(10)
    # create a marker
    marker = map_widget.set_marker(51.75872785,-1.26279873, text="Location")
    update()
    window.mainloop()