import serial

ser = serial.Serial('/dev/ttyS0', 9600)

while True:
	leerSerial = ser.read_until(b"\n")
	print("serial Xbee")
	print(leerSerial)