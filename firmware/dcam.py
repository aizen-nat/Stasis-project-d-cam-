from gpiozero import LED, Buzzer
from mfrc522 import SimpleMFRC522
from RPLCD.i2c import CharLCD
from adafruit_fingerprint import Adafruit_Fingerprint
from picamera2 import Picamera2
from datetime import datetime
import serial
import os
import time

green = LED(17)
red = LED(27)
blue = LED(22)

relay = LED(23)

buzzer = Buzzer(24)

reader = SimpleMFRC522()

lcd = CharLCD(
    i2c_expander='PCF8574',
    address=0x27,
    port=1,
    cols=16,
    rows=2
)

finger_uart = serial.Serial("/dev/ttyUSB0", baudrate=57600, timeout=1)

finger = Adafruit_Fingerprint(finger_uart)

camera = Picamera2()

camera.configure(camera.create_still_configuration())

camera.start()

exam_mode = True

log_path = "/home/pi/attendance_logs.txt"

photo_dir = "/home/pi/captures"

os.makedirs(photo_dir, exist_ok=True)

def clear_leds():
    green.off()
    red.off()
    blue.off()

def tone(count, speed):
    for _ in range(count):
        buzzer.on()
        time.sleep(speed)
        buzzer.off()
        time.sleep(0.08)

def screen(a, b):
    lcd.clear()
    lcd.write_string(a)
    lcd.cursor_pos = (1, 0)
    lcd.write_string(b)

def save_log(uid, name, state):
    now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    line = f"{now} | {uid} | {name} | {state}\n"

    with open(log_path, "a") as f:
        f.write(line)

def snap(uid):
    path = f"{photo_dir}/{uid}.jpg"

    camera.capture_file(path)

def verify_fingerprint():
    while finger.get_image() != adafruit_fingerprint.OK:
        pass

    if finger.image_2_tz(1) != adafruit_fingerprint.OK:
        return False

    if finger.finger_search() != adafruit_fingerprint.OK:
        return False

    return True

def granted(uid, name):
    green.on()

    relay.on()

    tone(2, 0.09)

    screen("Access Granted", name[:16])

    snap(uid)

    save_log(uid, name, "APPROVED")

    time.sleep(4)

    relay.off()

    clear_leds()

    screen("Attendance Sys", "Scan RFID Card")

def denied():
    relay.off()

    screen("Access Denied", "Try Again")

    for _ in range(3):
        red.on()
        tone(1, 0.16)
        red.off()
        time.sleep(0.1)

    save_log("UNKNOWN", "UNKNOWN", "DENIED")

    clear_leds()

    time.sleep(2)

    screen("Attendance Sys", "Scan RFID Card")

screen("Attendance Sys", "Scan RFID Card")

while True:

    try:

        blue.on()

        uid, text = reader.read()

        uid = str(uid)

        name = text.strip()

        blue.off()

        if not name:
            denied()
            continue

        if exam_mode:

            screen("Place Finger", "Verification")

            if verify_fingerprint():
                granted(uid, name)

            else:
                denied()

        else:
            granted(uid, name)

    except KeyboardInterrupt:
        lcd.clear()
        clear_leds()
        break

    except Exception:
        denied()