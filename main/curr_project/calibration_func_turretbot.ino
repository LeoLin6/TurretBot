#include <AccelStepper.h>
const int Pin=8; //for induction sensor
const int pitch_pin = 11; //pitch induction sensor
AccelStepper stepper1(1, 2, 5);
AccelStepper pitch_motor(1,3,6); 

void setup() {
    pinMode(Pin, INPUT);
    pinMode(pitch_pin, INPUT);
    //Serial.begin(384000);
    stepper1.setMaxSpeed(2000);
    stepper1.setSpeed(2000); 
    pitch_motor.setMaxSpeed(100000);
    pitch_motor.setSpeed(200); 
}
 
void loop() {
    //exit(0);
    //stepper1.runSpeed();

    run_calibration_seq();
    pitch_calibration();
    
}

void run_calibration_seq(){
    while(digitalRead(Pin)!=HIGH){
      stepper1.runSpeed();
      Serial.println("no object");
    }
    
    Serial.println("Object detected");
    stepper1.stop();
}

void pitch_calibration(){
    while(digitalRead(pitch_pin)!=HIGH){
      pitch_motor.runSpeed();
      Serial.println("no object");
    }
    
    Serial.println("Object detected");
    pitch_motor.stop();
}

void stop() {
  exit(0);
}