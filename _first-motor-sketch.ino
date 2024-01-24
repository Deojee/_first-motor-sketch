#include <AccelStepper.h>
#include "StepperMotor.h"
#include <Servo.h>


const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
char messageFromPC[numChars] = {0};
boolean newData = false;




const int arraySize = 6; // Define the size of the array
float values[arraySize]; // Array to store the floats

StepperMotor baseMotor(AccelStepper(1, 4, 3), 200 * (3/1) * (6/1), -3600, 3600);

//1, step pin, dir pin
StepperMotor shoulderMotor(AccelStepper(1, 10, 9), -200 * 2 * (76.0/18.0) * (75.0/25.0), -90, 90);  // Example pin numbers, gear ratio, min and max angles
        // Pin 8 and 9 are connected to the stepper motor driver
        // 1.8 is the gear ratio (e.g., 1.8° per step)
        // Min angle is 0 degrees, max angle is 360 degrees

StepperMotor elbowMotor(AccelStepper(1, 12, 11), -200 * 2 * (76.0/18.0) * (90.0/30.0), -360, 360);

StepperMotor elbowTwistMotor(AccelStepper(1, 8, 7), -200 * 2 * (72.0/18.0) * (72.0/18.0), -360, 360);

StepperMotor wristMotor(AccelStepper(1, 6, 5), -200 * 2 * (80.0/20) * (70.0/20), -360, 360);

Servo wristTwist;

void setup() {

  Serial.begin(38400);
  Serial.setTimeout(200);

  wristTwist.attach(2);


  //stepper1.moveToAngle(360);
  //stepper2.moveToAngle(360);
}



void loop() {

  recvWithStartEndMarkers();

  if (newData == true){

    strcpy(tempChars, receivedChars);
    parseData();
    useParsedData();

    newData = false;
  }

  baseMotor.run();
  shoulderMotor.run();
  elbowMotor.run();
  elbowTwistMotor.run();
  wristMotor.run();

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

        //Serial.print(i); Serial.print(": "); Serial.print(values[i]); Serial.print(" ");

    }//Serial.println("~");
}

void useParsedData() {

    baseMotor.moveToAngle(values[0]);
    shoulderMotor.moveToAngle(values[1]);
    elbowMotor.moveToAngle(values[2] + ((1.0/3.0) * values[1]));

    elbowTwistMotor.moveToAngle(values[3]);
    wristMotor.moveToAngle(values[4]);

    wristTwist.write(values[5] * (180.0/300.0));
    
}