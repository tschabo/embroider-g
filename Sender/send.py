import serial
import pyembroidery
import time

def sendOne(ser, s, mot):
    cmmd = ">m{};{};{};500.0;".format(s[0]/10, -s[1]/10, int(mot))
    print(cmmd)
    ser.write(cmmd.encode())
    ser.flush()
    bytes_to_read = ser.inWaiting()
    # wait for response to be send from device
    while bytes_to_read < 1:
        time.sleep(0.05)
        bytes_to_read = ser.inWaiting()
    x = ser.read()
    print(x)
    if x.decode() == "!":
        bytes_to_read = ser.inWaiting()
        # wait for response to be send from device
        while bytes_to_read < 1:
            time.sleep(0.05)
            bytes_to_read = ser.inWaiting()
        x = ser.read()
        print(x)


ser = serial.Serial()
ser.port = "/dev/ttyUSB0"
ser.baudrate = 115200
ser.open()
time.sleep(3)
while not ser.is_open:
    print(666)
ser.write(">e".encode())
bytes_to_read = ser.inWaiting()
# wait for response to be send from device
while bytes_to_read < 1:
    time.sleep(0.05)
    bytes_to_read = ser.inWaiting()
print(ser.read())
pattern=pyembroidery.read_pes("./samples011214-07.pes")
for s in pattern.stitches:
    sendOne(ser, s, ((200/4)*16))
    sendOne(ser, s, ((200/4)*16*3))
