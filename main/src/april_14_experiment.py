# Importing Libraries 
import serial 
import time 
arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1) 

#global variables
num_of_exp_one = 72
rps = 0.05

def calibrate():
        print ("running calibration sequence")
        arduino.write(b"cali\n")
        arduino.write(bytes('16\n', 'utf-8'))
        time.sleep(7)
        print ("calibration complete")

def experiment_one (): #Automatically move talking head in increments of 5deg (multiple of the standard 15deg)
        time.sleep(2)   # arduino setup time
        # calibrate first
        calibrate()
        #next lines for 5 degrees movement
        i = 0
        while i< num_of_exp_one:
                i+=1
                print ("We are on movement step number: ", i)
                arduino.write(b"pos\n")
                arduino.write(bytes('5\n', 'utf-8'))
                time.sleep(5)
                # can play audio here
                experiment_one_audio()

def experiment_one_audio():
    #empty
    time.sleep(3)

def experiment_two_audio():
    #WGN
    time.sleep(4)

def experiment_two():
    time.sleep(2)   # arduino setup time
    # calibrate first
    calibrate()
    
    arduino.write(b"pos\n")
    arduino.write(bytes('90\n', 'utf-8'))
    time.sleep(1)
    print("moved to +90")
    experiment_two_audio()
    print("experiment now")
    arduino.write(b"vel\n")
    arduino.write(bytes('5\n', 'utf-8'))
    arduino.write(bytes('10\n', 'utf-8'))
    print(str(1/(2*rps)))
    time.sleep(10)
    print("moved to -90")
    
                

#run code here
#experiment_one()
experiment_two()
