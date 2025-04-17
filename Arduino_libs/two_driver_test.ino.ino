#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>

/* README: procedure:
For some specified payload motor speed (this is what is being measured acoustically):
1. Initialize ang=0. Move to ang
2. Run payload motor at speed S for T seconds
3. ang += ang_step
4. Repeat

Run the above for various speeds S as specified below

AUDIO CUES:
1 chirp: end of one azimuth step
2 chirps: end of calibration sequence (homing procedure)
3 chirps: end of one full revolution in azimuth
*/

#define HOMING_PIN 8 //for induction sensor
#define ALARM_PIN 6 // for whatever sound-making sensor is
#define TEST_SERVO_PIN 9
#define PAUSE_CHIRP 500 // ms
#define PAUSE_SHORT 200 // ms
#define PAUSE_LONG 30000 // ms
#define PAUSE_MEASURE 10000 // ms, how long to run the test motor per pose
#define PAUSE_NOISEONLY 10000 // ms

#define TABLE_MAXSPEED 2000 // steps per sec
#define TABLE_SPEED 500 // steps per sec
#define TABLE_STEPS_PER_REV 6400
#define TABLE_NUM_ANGLES 24 // corresponds to a resolution of 15 deg.

#define TEST_MAX_SPEED 2000
#define TEST_STEPS_PER_REV 6400
#define TEST_MINSPR 2 // fastest (SPR = seconds per Revolution)
#define TEST_INCSPR 2 // so for each `experiment` call, increment speed `secperrev` by this much
#define TEST_NUM_SPR 5

// GLOBAL VARS
// Define the stepper motor and the HOMING_PINs that is connected to
AccelStepper tableMotor(1, 2, 5); //turntable motor
AccelStepper testMotor(1, 3, 4); //experiement motor
Servo testServo;
Servo alarm;

void setup() {
  // turntable setup
  tableMotor.setMaxSpeed(TABLE_MAXSPEED);
  tableMotor.setSpeed(TABLE_SPEED); // Set maximum speed value for the stepper

  // alarm setup
  alarm.attach(ALARM_PIN);
  alarm.write(0); //reset servo to position 0
 
  // run away!!!!
  delay(PAUSE_LONG);
  doAlarmChirp(4);
  delay(PAUSE_NOISEONLY);
  doAlarmChirp(4);
}

void loop() {
  for (int n=0; n<TEST_NUM_SPR; n++) {
    int secs_per_rev = TEST_MINSPR + n * TEST_INCSPR;
    experiment(secs_per_rev);
  }
}

void experiment(int secs_per_rev){
  // test motor setup
  testMotor.setMaxSpeed(TEST_STEPS_PER_REV);
  testMotor.setSpeed(TEST_STEPS_PER_REV / secs_per_rev);
  
  // reset turntable
  TableToHome();
  tableMotor.setCurrentPosition(0);
  tableMotor.setSpeed(TABLE_SPEED);
  doAlarmChirp(2);

  for (int step=0; step<TABLE_NUM_ANGLES; step ++) {

    // do one discrete step
    int tableStep1 = int(TABLE_STEPS_PER_REV/TABLE_NUM_ANGLES*step);
    while(tableMotor.currentPosition()<=tableStep1){ tableMotor.runSpeed(); }
    delay(PAUSE_SHORT);

    doAlarmChirp(1);
    delay(PAUSE_SHORT);

    //play test motor for 10 seconds
    unsigned long t1 = millis()+PAUSE_MEASURE;
    testServo.attach(TEST_SERVO_PIN);
    testServo.write(180);
    while(millis() < t1){ 
      testMotor.runSpeed();
    }
    testServo.detach();
    delay(PAUSE_SHORT);

    }
  // CABLE MANAGEMENT
  doAlarmChirp(3);

  tableMotor.setSpeed(-TABLE_SPEED);
  while(tableMotor.currentPosition()>-100){
    tableMotor.runSpeed();
  }
  tableMotor.setSpeed(TABLE_SPEED);
}

void TableToHome(){
    while(digitalRead(HOMING_PIN)!=HIGH){
      tableMotor.runSpeed();
      Serial.println("no object");
    }
    // tableMotor.stop();
    tableMotor.setCurrentPosition(0);
    tableMotor.setSpeed(TABLE_SPEED);
    // tableMotor.runToNewPosition(100);

    Serial.println("Object detected");
    //delay(1000);
    
}

void doAlarmChirp(int num){
  for (int i = 0; i < num; i++) {
    delay(PAUSE_SHORT);
    alarm.write(180);
    delay(PAUSE_SHORT);
    alarm.write(0);
    delay(PAUSE_SHORT);
  }
  delay(PAUSE_CHIRP);  
}