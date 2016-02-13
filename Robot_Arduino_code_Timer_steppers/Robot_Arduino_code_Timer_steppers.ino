////////////////////////////////////////////////////////////////////////////////
//    Library Includes
////////////////////////////////////////////////////////////////////////////////

#include "TimerOne.h"
#include "steppers.h"
//#include "AccelStepper.h"

////////////////////////////////////////////////////////////////////////////////
//    Pin Defines
////////////////////////////////////////////////////////////////////////////////

//    IR Sensor Pins
#define IR_RIGHT  A0
#define IR_CENTER A1
#define IR_LEFT   A2

//    DC Motor Pins
#define MOTOR_RIGHT 11
#define MOTOR_LEFT  10

////////////////////////////////////////////////////////////////////////////////
//    Packet Designations
////////////////////////////////////////////////////////////////////////////////

#define IR_RIGHT_TAG  0x8000
#define IR_CENTER_TAG 0x8600
#define IR_LEFT_TAG   0x8800

#define MOTOR_LEFT_TAG  0x4C
#define MOTOR_RIGHT_TAG 0x52

////////////////////////////////////////////////////////////////////////////////
//    Global Variables
////////////////////////////////////////////////////////////////////////////////
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
unsigned int sensorValue = 0;
unsigned char cmdTarget = 0;

////////////////////////////////////////////////////////////////////////////////
//    Global Objects
////////////////////////////////////////////////////////////////////////////////
//AccelStepper leftStepper(1, STEPPER_LEFT_STEP, STEPPER_LEFT_DIR, STEPPER_LEFT_MS1, STEPPER_LEFT_MS2);
//AccelStepper rightStepper(1, STEPPER_RIGHT_STEP, STEPPER_RIGHT_DIR, STEPPER_RIGHT_MS1, STEPPER_RIGHT_MS2);

////////////////////////////////////////////////////////////////////////////////
//    Setup Function
////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  inputString.reserve(2);

  // DC Motor Pins
  pinMode(MOTOR_LEFT, OUTPUT);
  pinMode(MOTOR_RIGHT, OUTPUT);
  
  initializeSteppers();
  
  
  /*leftStepper.setMaxSpeed(1000);
  leftStepper.setSpeed(0);

  rightStepper.setMaxSpeed(1000);
  rightStepper.setSpeed(0);
  
  Timer1.initialize(1000); // 500,000 uS = .5 S
  //Timer1.attachInterrupt(updateSensors);*/
}

////////////////////////////////////////////////////////////////////////////////
//    Main Loop
////////////////////////////////////////////////////////////////////////////////

void loop() {  
  if (stringComplete)
  {
    cmdTarget = inputString[0];
    unsigned char cmdValue = inputString[1];
    //Serial.println(cmdValue);
    noInterrupts();
    if (cmdTarget == MOTOR_LEFT_TAG)
    {
      if(cmdValue != 127)
        setLeftStepperRPM(map(cmdValue, 0, 255, -MAX_SPEED, MAX_SPEED));
      else
        setLeftStepperRPM(0);
      //analogWrite(MOTOR_LEFT, inputString[1]); //PWM DC Motor
    }
    else if (cmdTarget == MOTOR_RIGHT_TAG)
    {
      if(cmdValue != 127)
        setRightStepperRPM(map(cmdValue, 0, 255, -MAX_SPEED, MAX_SPEED));
      else
        setRightStepperRPM(0);
      //analogWrite(MOTOR_RIGHT, inputString[1]); //PWM DC Motor
    }
    else
      Serial.println("FML");
    interrupts();

    stringComplete = false;
    inputString = "";
  }
  //leftStepper.runSpeed();
  //rightStepper.runSpeed();
  //leftStepper.runSpeed();
  //rightStepper.runSpeed();
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  if (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    while (!Serial.available()) {}
    inChar = (char)Serial.read();
    inputString+= inChar;
    //Serial.println(inputString);
    stringComplete = true;
  }
}

void updateSensors()
{
  sensorValue = analogRead(IR_RIGHT);
  sensorValue |= IR_RIGHT_TAG;
  Serial.println(sensorValue);
  sensorValue = analogRead(IR_CENTER);
  sensorValue |= IR_CENTER_TAG;
  Serial.println(sensorValue);
  sensorValue = analogRead(IR_LEFT);
  sensorValue |= IR_LEFT_TAG;
  Serial.println(sensorValue);
}

