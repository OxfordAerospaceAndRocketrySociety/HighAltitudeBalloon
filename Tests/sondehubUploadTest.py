from sondehub.amateur import Uploader
import time
import logging 

if __name__ == "__main__":
    #Enable debug output
    logging.basicConfig(format="%(asctime)s %(levelname)s:%(message)s", level=logging.INFO)
    uploader = Uploader(
    "OXHABTRACK0",
    developer_mode=True)
    #Demo telemetry
    telemetry = [0,"OXHABTEST0","1","09:12:30","51.75872785","-1.26279873","60","FFFF"]
    uploader.add_telemetry(
        telemetry[1], #Callsign
        telemetry[3], #Time
        float(telemetry[4]), #Latitude
        float(telemetry[5]), #Longitude
        int(telemetry[6]), #Altitude,
        int(telemetry[2]) #Packet count
    )
    time.sleep(3)
    uploader.close()
