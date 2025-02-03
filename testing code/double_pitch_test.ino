#include <AccelStepper.h>
#include <MultiStepper.h>

// Define the stepper motor and the pins that is connected to
AccelStepper stepper1(1, 2, 5); // (Typeof driver: with 2 pins, STEP, DIR)
AccelStepper stepper2(1, 3, 6);

MultiStepper steppersControl;  // Create instance of MultiStepper

long gotoposition[2]; // An array to store the target positions for each stepper motor

void setup() {

  stepper1.setMaxSpeed(500); // Set maximum speed value for the stepper
  stepper2.setMaxSpeed(500);
  stepper2.setAcceleration(0.01); 
  stepper1.setAcceleration(0.01); 

  // Adding the 3 steppers to the steppersControl instanc
//for multi stepper control
  steppersControl.addStepper(stepper1);
  steppersControl.addStepper(stepper2);
}

void loop() {

  //delay(10000000000);
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  // Store the target positions in the "gotopostion" array
  gotoposition[0] = 300;  // 800 steps - full rotation with quater-step resolution
  gotoposition[1] = -300;

  steppersControl.moveTo(gotoposition); // Calculates the required speed for all motors
  steppersControl.runSpeedToPosition(); // Blocks until all steppers are in position

  delay(3000);

  gotoposition[0] = 0;
  gotoposition[1] = 0;

  steppersControl.moveTo(gotoposition);
  steppersControl.runSpeedToPosition();

  delay(1000);
  //
}