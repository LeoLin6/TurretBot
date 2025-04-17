// -----------------------------
// Pin Definitions
// -----------------------------
#define STEP_PIN    10
#define RX_PIN      2
#define TX_PIN      3
#define EN_PIN      12
#define SENSOR_PIN  A1 // Inductive sensor

// -----------------------------
// Configuration Constants
// -----------------------------
#define CURRENT       726
#define MICROSTEP     0  // Used for driver.mres() if needed
const int microsteps = 64;
const double stepAngle = 360.0 / (200.0 * microsteps); // degrees per microstep
const int pulseWidth = 200; // microseconds

// -----------------------------
// Library Includes
// -----------------------------
#include <TMC2208Stepper.h>
#include <SoftwareSerial.h>

// -----------------------------
// Global Objects and Variables
// -----------------------------
SoftwareSerial mySerial(RX_PIN, TX_PIN);
TMC2208Stepper driver = TMC2208Stepper(&mySerial);
float duration;
float x;

// -----------------------------
// Setup Function
// -----------------------------
void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);

  driver.pdn_disable(1);                // Use UART communication
  driver.I_scale_analog(0);             // Use digital current control
  driver.rms_current(CURRENT);          // Set current
  driver.toff(5);                        // Enable driver (low time)
  driver.en_spreadCycle(0);             // Use stealthChop mode
  driver.pwm_autoscale(1);              // Enable voltage regulator
  driver.mstep_reg_select(1);
  driver.microsteps(microsteps);        // Set microstep resolution

  driver.push(); // Apply settings
}

// -----------------------------
// Loop Function
// -----------------------------
void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "cali") {
      x = Serial.readStringUntil('\n').toFloat();
      run_calibration_seq(x);

    } else if (cmd == "pos") {
      x = Serial.readStringUntil('\n').toFloat();
      MoveToPosition(x);

    } else if (cmd == "vel") {
      float rps = Serial.readStringUntil('\n').toFloat() / 100.0;
      float dur = Serial.readStringUntil('\n').toFloat() / 100.0;
      run_rps_for_duration(rps, dur);
    }

    Serial.println(x); // Echo value
  }
}

// -----------------------------
// Motion & Control Functions
// -----------------------------

// Move a specific number of degrees
void MoveToPosition(double degrees) {
  bool backwards = false;
  
  if (degrees < 0) {
    degrees *= -1;
    driver.shaft(1);  // Reverse direction
    backwards = true;
  }

  long steps = degrees / stepAngle;
  for (long i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(pulseWidth+1000);
  }

  if (backwards) driver.shaft(0); // Restore direction
}

// Run motor at a constant RPS
void run_rps(double rps) {
  driver.VACTUAL(rps / 0.715 * 256 * 200); // Empirical conversion
}

// Run motor for specific duration at RPS
void run_rps_for_duration(double rps, double duration) {
  const int stepsPerRev = 200;
  const int microstepsPerRev = stepsPerRev * microsteps;
  const double usecPerStep = 1000000.0 / (rps * microstepsPerRev);
  const unsigned long totalSteps = rps * duration * microstepsPerRev;

  unsigned long start_time = micros();
  double nextStepTime = start_time;

  for (unsigned long i = 0; i < totalSteps; i++) {
    while ((double)micros() < nextStepTime);
    
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(STEP_PIN, LOW);

    nextStepTime += usecPerStep;
  }

  delay(1000); // Optional delay
}

// Accelerate to a target RPS in ~1s
void AccelToSpeed(double rps) {
  for (double i = 0; i <= rps; i += rps / 100) {
    run_rps(i);
    delay(10);
  }
}

// Decelerate from current RPS to 0 in ~1s
void DecelFromSpeed(double rps) {
  for (double i = rps; i >= 0; i -= rps / 100) {
    run_rps(i);
    delay(10);
  }
}

// Run calibration sequence until sensor is triggered
void run_calibration_seq(double delayTime) {
  while (digitalRead(SENSOR_PIN) != HIGH) {
    run_rps(0.05);
  }

  delay(delayTime);
  run_rps(0);
  delay(100);
}
