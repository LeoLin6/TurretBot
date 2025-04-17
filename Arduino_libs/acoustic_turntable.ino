#include <AccelStepper.h>
#include <MultiStepper.h>
//#include <TimeLib.h> 

// Define the stepper motor and the pins that is connected to
AccelStepper t(1, 2, 5); //turntable motor
AccelStepper A(1, 3, 6); //experiement motor
unsigned long endTime;
int target_p;

MultiStepper steppersControl;  // Create instance of MultiStepper

long gotoposition[2]; // An array to store the target positions for each stepper motor

void setup() {
  t.setMaxSpeed(2000);
  t.setSpeed(500); // Set maximum speed value for the stepper
  A.setMaxSpeed(500);
  A.setSpeed(500);
  // Adding steppers to the steppersControl instance for multi stepper control
  //steppersControl.addStepper(t);
  //steppersControl.addStepper(A);

  //t.setCurrentPosition(0);
  target_p = 300;
}

void loop() {
  //while(true){}
  experiment(target_p);
  target_p +=300;
  if(target_p >= 2400){
    while(true){}
  }
  
}

void experiment(int target){
  //gotoposition[0] = 300;  

  while(t.currentPosition()<target){
    t.runSpeed();
  }

  delay(3000);

  endTime = millis()+10000; //10 seconds
  //play motor A for 10 seconds
  while(millis() < endTime){ 
    A.runSpeed();
  }
}