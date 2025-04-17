#include <AccelStepper.h>
#include <MultiStepper.h>

// Define the stepper motor and the pins that is connected to
 // (Typeof driver: with 2 pins, STEP, DIR)
AccelStepper stepper2(1,10
, 9);

MultiStepper steppersControl;  // Create instance of MultiStepper

long gotoposition[1]; // An array to store the target positions for each stepper motor

void setup() {

  // Set maximum speed value for the stepper
  stepper2.setMaxSpeed(5000);
  stepper2.setSpeed(500); 

  // Adding the 3 steppers to the steppersControl instanc
//for multi stepper control
  
  steppersControl.addStepper(stepper2);
}

void loop() {

  //delay(10000000000);
  // Store the target positions in the "gotopostion" array
  gotoposition[0] = 5000;  // 800 steps - full rotation with quater-step resolution

  steppersControl.moveTo(gotoposition); // Calculates the required speed for all motors
  steppersControl.runSpeedToPosition(); // Blocks until all steppers are in position

  delay(3000);

  gotoposition[0] = -5000;

  steppersControl.moveTo(gotoposition);
  steppersControl.runSpeedToPosition();

  delay(1000);
  //
}