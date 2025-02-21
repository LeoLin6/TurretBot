// Defining
//using software serial to control tmc 2208 chip through UART
int x;
#define StepPin     10
#define RX_PIN      2
#define TX_PIN      3
#define EN          12
#define Pin       A1 //for induction sensor



#define Current     1000
#define MicroStep   0     // 0: Native 256 microstep setting.
                          // 1: 128 microsteps per full step.
                          // 2: 64 microsteps per full step.
                          // 3: 32 microsteps per full step.
                          // 4: 16 microsteps per full step.
                          // 5: 8 microsteps per full step.
                          // 6: 4 microsteps per full step.
                          // 7: 2 microsteps per full step.
                          // 8: Full step.
                    
const double stepAngle = 360.0 / (200.0 * 256.0); 

// Including library
#include <TMC2208Stepper.h>                                // Include library
#include <SoftwareSerial.h> 

SoftwareSerial mySerial(RX_PIN, TX_PIN);
TMC2208Stepper driver = TMC2208Stepper(&mySerial);    // Create driver and use

// Setup
void setup() {
  //driver.beginSerial(115200); 
  mySerial.begin(115200);  
  driver.push();
  Serial.begin(115200);

  // Prepare pins
  pinMode(StepPin, OUTPUT);
  pinMode(EN, OUTPUT);

  driver.pdn_disable(1);              // Use PDN/UART pin for communication
  driver.I_scale_analog(0);           // Adjust current from the registers
  driver.rms_current(Current);        // Set driver current 
  driver.toff(5);                     // Enable driver
  //digitalWrite(EN, LOW);
  
  driver.en_spreadCycle(0);           // 1: spreadCycle
                                      // 0: stealthChop
  driver.pwm_autoscale(1);            // 1: if stealthChop is chosen. Otherwise 0
  driver.mstep_reg_select(1);
  driver.mres(MicroStep); 
  run_calibration_seq();
/*
  AccelToSpeed(1.0);
  run_rps(1.0);
  delay(10000);
  DecelFromSpeed(1);*/
}

void MoveToPosition(double degrees){
  bool backwards = 0;
  if(degrees<0){
    degrees = degrees * -1;
    driver.shaft(1);
    //delayMicroseconds(100);
    backwards = 1;
  }
  long steps = degrees/stepAngle; // Convert degrees to steps
  
  for (long i = 0; i < steps; i++) {
    //Serial.print(steps);
    digitalWrite(StepPin, HIGH); // Create a step pulse
    delayMicroseconds(100); // Pulse width
    digitalWrite(StepPin, LOW); // End pulse
    delayMicroseconds(100); // Delay between steps
  }
  if(backwards)
    driver.shaft(0); // switch back to normal direction

}

//runs motor at desired rps
void run_rps(double rps){
  driver.VACTUAL(rps / 0.715 * 256 * 200);
}

//takes rps as input, accelerates to rps speed from 0rps in 1.0seconds. 
void AccelToSpeed(double rps){
  for(double i=0; i<rps; i+= rps/100){
    run_rps(i);
    delay(10);
  }
}

//takes rps as input decelerates to 0 from that rps value in 1.0seconds. 
void DecelFromSpeed(double rps){
  for(double i=rps; i>=0; i-= rps/100){
    run_rps(i);
    delay(10);
  }
}
// Looping
void loop() {
  while (!Serial.available()); 
  String str = Serial.readStringUntil('\n');
  if(str=="cali")               //home in seq if input is a 1
    run_calibration_seq();
  else {
	  x = Serial.readString().toFloat(); 
    MoveToPosition(x);
  }
  
  
	Serial.print(x); 
}

void run_calibration_seq(){
    while(digitalRead(Pin)!=HIGH){
      run_rps(0.2);
      //Serial.println("no object");
    }
    delay(67);
    //Serial.println("Object detected");
    run_rps(0);
}
