
import pyrebase
import gpiozero
import time

trigger = gpiozero.OutputDevice(14)
echo = gpiozero.InputDevice(15)

config = {
    "authDomain": "iot-2020umng.firebaseapp.com",
    "databaseURL": "https://iot-2020umng.firebaseio.com/",
    "apiKey": "TPAksSAwOnNnGAD445el8lQDGpglK7Ntr204QOtu",
    "storageBucket": "",
}

KEY_PATH = "/Goldberg/moduloB"
DIST_FOR_TRIGGER = 10 # cm

def main():
    firebase = pyrebase.initialize_app(config)
    database = firebase.database()

    finished = False
    while not finished:
        print("starting")
        trigger.value = False
        time.sleep(0.1)
        trigger.value = True
        start_time = time.clock()
        waiting = True
        timeout = False
        while waiting:
            if echo.value:
                waiting = false
            elif time.clock() - start_time > 1.0:
                timeout = True
                waiting = False
        if timeout:
            print('ultrasonido timeout')
            continue
        end_time = time.clock()
        round_trip_time = end_time - start_time
        speed_of_sound = 34029 # in centimeters per second
        round_trip_dist = round_trip_time * speed_of_sound
        dist = round_trip_dist * 0.5
        print("got dist: %s" % dist)
        if dist < DIST_FOR_TRIGGER:
            entry = database.child(KEY_PATH)
            data = entry.set(1)
            print("writing %s with value %s" % (KEY_PATH, data))
        time.sleep(0.1)


if __name__ == "__main__":
    main()
