import colorextract, os, serial
import serial

arduino_port = '/dev/tty.usbmodem12341';
def get_arduino_serial():
	s = serial.Serial(arduino_port, 9600)
	while 1:
		try:
			if s.inWaiting():
				val = s.read(s.inWaiting())
				if(val == 'ON'):
					take_picture();
		except serial.serialutil.SerialException:
			print s.inWaiting()

def take_picture():
	os.system('./webcamsave')

def extract_colors():
	print colorz('input.jpg')

def main():
	get_arduino_serial()

if  __name__ =='__main__':main()