#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include <AccelStepper.h>

class StepperMotor {
private:
    AccelStepper stepper;
    int minAngle;
    int maxAngle;
    float stepsPerRevolution;

public:
    StepperMotor(AccelStepper newStepper, float stepsPerRevolution, int minAngle, int maxAngle);
    void moveToAngleRads(float angle);
    void moveToAngle(float angle);
    void run();
};

#endif