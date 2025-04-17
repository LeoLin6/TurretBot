// Defining
//using software serial to control tmc 2208 chip through UART
int x;
#define StepPin     10
#define RX_PIN      2
#define TX_PIN      3
#define EN          12
#define Pin         A1 //for induction sensor



#define Current     726
#define MicroStep   0     // 0: Native 256 microstep setting.
                          // 1: 128 microsteps per full step.
                          // 2: 64 microsteps per full step.
                          // 3: 32 microsteps per full step.
                          // 4: 16 microsteps per full step.
                          // 5: 8 microsteps per full step.
                          // 6: 4 microsteps per full step.
                          // 7: 2 microsteps per full step.
                          // 8: Full step.

const int microsteps = 64;                
const double stepAngle = 360.0 / (200.0 * microsteps); 
const int pulseWidth = 200; // microseconds


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
  //driver.mres(MicroStep); 
  //run_calibration_seq();
  driver.microsteps(microsteps);
  //delay(1000);
  //run_rps_for_duration(0.05, 10);
/*
  AccelToSpeed(1.0);
  run_rps(1.0);
  delay(10000);
  DecelFromSpeed(1);
*/}

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
    delayMicroseconds(pulseWidth); // Pulse width
    digitalWrite(StepPin, LOW); // End pulse
    delayMicroseconds(pulseWidth); // Delay between steps
  }
  if(backwards)
    driver.shaft(0); // switch back to normal direction

}

//runs motor at desired rps
void run_rps(double rps){
  driver.VACTUAL(rps / 0.715 * 256 * 200);
}
/*
void run_rps_for_duration(double rps, double duration) {
  driver.shaft(0);
  const int steps_per_rev = 200;
  const int microsteps = 2;
  const int pulseWidth = 100; // in microseconds — safe default

  // Total microsteps per revolution
  const int microsteps_per_rev = steps_per_rev * microsteps;

  // Time between steps
  double usec_per_step = 1000000.0f / (float(microsteps_per_rev));

  // Total number of steps to run
  unsigned long total_steps = rps * duration * microsteps_per_rev;

  for (unsigned long i = 0; i < total_steps; i++) {
    digitalWrite(StepPin, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(StepPin, LOW);
    delayMicroseconds(usec_per_step/rps - pulseWidth);
  }
  delay(1000);
}*/

void run_rps_for_duration(double rps, double duration) {
  //driver.shaft(0); // Set direction

  const int steps_per_rev = 200;
  
  

  const int microsteps_per_rev = steps_per_rev * microsteps;
  const double usec_per_step = 1000000.0f / (rps * microsteps_per_rev);
  const unsigned long total_steps = rps * duration * microsteps_per_rev;

  unsigned long start_time = micros();
  double next_step_time = start_time;

  for (unsigned long i = 0; i < total_steps; i++) {
    // Wait until it's time for the next step
    while ((double)micros() < next_step_time);

    digitalWrite(StepPin, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(StepPin, LOW);

    next_step_time += usec_per_step;
  }

  // Optional pause
  delay(1000);
}

/*

void run_rps_for_duration (double rps, double time){
  run_rps(rps);
  delay(time*1000);
  run_rps(0);
}*/

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
float duration;
void loop() {
  if (Serial.available()){
    String str = Serial.readStringUntil('\n');
    str.trim();
     

    if(str=="cali"){               //home in seq if input is a 1
    x = Serial.readStringUntil('\n').toFloat();
      run_calibration_seq(x);
    }else if (str == "pos"){
      //x = Serial.readString().toFloat(); 
      x = Serial.readStringUntil('\n').toFloat();
      MoveToPosition(x);
    } else if (str == "vel"){
      x = Serial.readStringUntil('\n').toFloat();
      duration = Serial.readStringUntil('\n').toFloat(); 
      run_rps_for_duration(x/100.0, 10);
      //run_rps_for_duration(x, duration);
    }
    
    
    Serial.print(x); 
  }
}

void run_calibration_seq(double del){
    while(digitalRead(Pin)!=HIGH){
      run_rps(0.05);
      //Serial.println("no object");
    }
    delay(del);
    //Serial.println("Object detected");
    run_rps(0);
    delay (100);
}
