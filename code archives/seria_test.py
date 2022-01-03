import serial
#type $ ls /dev/ttty* in terminal of RPi
#figure out the port
#https://classes.engineering.wustl.edu/ese205/core/index.php?title=Serial_Communication_between_Raspberry_Pi_%26_Arduino
ser = serial.Serial('/dev/',9600)
s = [0,1]
while True:
    read_serial=ser.readline()
    s[0] = str(int (ser.readline(),16))
    print s[0]
    print read_serial
