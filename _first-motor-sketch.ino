#include <AccelStepper.h>
#include "StepperMotor.h"


const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
char messageFromPC[numChars] = {0};
boolean newData = false;


// Pins for stepper motor control
const int stepPin1 = 7; // Step pin for stepper motor 1
const int dirPin1 = 6; // Direction pin for stepper motor 1

const int stepPin2 = 9;
const int dirPin2 = 8;

//const int stepPin2 = 10; // Step pin for stepper motor 2
//const int dirPin2 = 11; // Direction pin for stepper motor 2

const int arraySize = 6; // Define the size of the array
float values[arraySize]; // Array to store the floats

//AccelStepper stepper1(1, stepPin1, dirPin1); // Stepper motor 1
//AccelStepper stepper2(AccelStepper::HALF_STEP, stepPin2, dirPin2); // Stepper motor 2

//StepperMotor stepper1(AccelStepper(1, stepPin1, dirPin1), 1600, -360, 360);

StepperMotor stepper2(AccelStepper(1, stepPin2, dirPin2), 1600, -360, 360);  // Example pin numbers, gear ratio, min and max angles
        // Pin 8 and 9 are connected to the stepper motor driver
        // 1.8 is the gear ratio (e.g., 1.8° per step)
        // Min angle is 0 degrees, max angle is 360 degrees

    

void setup() {
  //pinMode(dirMinus,OUTPUT);
  //pinMode(pulsePlus,OUTPUT);
  //digitalWrite(dirMinus, HIGH);

  Serial.begin(38400);
  Serial.setTimeout(200);

  //values[1] = 0;

  //stepper1.setMaxSpeed(800.0); // Set maximum speed for stepper motor 1
  //stepper1.setAcceleration(500.0);

  //stepper2.setMaxSpeed(1000.0); // Set maximum speed for stepper motor 2
}



void loop() {
  
  /*
  if (Serial.available() > 0){
    Serial.read();
    useParsedData();
  }

  stepper2.run();

  return;*/

  //Serial.println("it works!");

  recvWithStartEndMarkers();

  if (newData == true){

    //Serial.println("it did a thing!");

    strcpy(tempChars, receivedChars);
    parseData();
    useParsedData();

    newData = false;
  }

  //stepper1.run();
  stepper2.run();

}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
    
    for (int i = 0; i < 6; i++){
        strtokIndx = strtok(NULL, ",");
        values[i] = atof(strtokIndx);     // convert this part to a float

        Serial.print(i); Serial.print(": "); Serial.print(values[i]); Serial.print(" ");

    }Serial.println("~");
}

void useParsedData() {

    //values[1] += 5;
    //stepper1.moveToAngle(values[0]);
    stepper2.moveToAngle(values[1]);
}