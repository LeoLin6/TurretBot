# Importing Libraries 
import serial 
import time 
arduino = serial.Serial(port='COM3', baudrate=115200, timeout=.1) 
'''
def write_read(x): 
	arduino.write(bytes(x, 'utf-8')) 
	time.sleep(0.05) 
	data = arduino.readline() 
	return data 
'''
while True: 
        string = input("Enter 1 for calibration, 2 for positional movement: ")
        if string =='1':
            arduino.write(b"cali\n")
        elif string=='2':
            num = input("Enter the position you want to move to(degrees): ") # Taking input from user
            arduino.write(b"pos\n")
            arduino.write(bytes(num, 'utf-8'))

        #print(value) # printing the value
                                   
    
