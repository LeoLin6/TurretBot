#include <AccelStepper.h>
const int Pin=8; //for induction sensor
AccelStepper stepper1(1, 3, 6); 

void setup() {
    pinMode(Pin, INPUT);
    //Serial.begin(384000);
    stepper1.setMaxSpeed(2000);
    stepper1.setSpeed(2000); 
}
 
void loop() {
    //exit(0);
    //stepper1.runSpeed();

    run_calibration_seq();
    
}

void run_calibration_seq(){
    while(digitalRead(Pin)!=HIGH){
      stepper1.runSpeed();
      Serial.println("no object");
    }
    
    Serial.println("Object detected");
    stepper1.stop();
}

void stop() {
  exit(0);
}