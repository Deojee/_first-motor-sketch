#include "StepperMotor.h"

StepperMotor::StepperMotor(AccelStepper newStepper, float stepsPerRevolution, int minAngle, int maxAngle){
    this->stepper = newStepper;
    this->minAngle = minAngle;
    this->maxAngle = maxAngle;
    this->stepsPerRevolution = stepsPerRevolution;
    stepper.setMaxSpeed(1000.0); // Set your maximum speed here
    stepper.setAcceleration(3000.0); // Set your acceleration here
    stepper.setCurrentPosition(0);
}

void StepperMotor::moveToAngleRads(float angle) {
  moveToAngle(angle * 57.2958);
}

void StepperMotor::moveToAngle(float angle) {
    if (angle < minAngle) {
        angle = minAngle;
    } else if (angle > maxAngle) {
        angle = maxAngle;
    }

    long steps = ((angle/360.0) * stepsPerRevolution); // 200 steps per revolution

    Serial.println(stepper.currentPosition());Serial.println(steps);

    stepper.moveTo(steps);
}

void StepperMotor::run(){
  stepper.run();
}
