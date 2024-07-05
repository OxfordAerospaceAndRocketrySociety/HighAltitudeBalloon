#Oxford Aerospace and Rocketry 2024
import tkinter
import tkintermapview

# create tkinter window
window = tkinter.Tk()
window.geometry(f"{500}x{500}")

# create map widget
map_widget = tkintermapview.TkinterMapView(window, width=500, height=500, corner_radius=0)
map_widget.pack()

# set position
map_widget.set_position(51.75872785,-1.26279873)
map_widget.set_zoom(14)

# create a marker
marker = map_widget.set_marker(51.76193215,-1.26171859, text="Location")

window.mainloop()