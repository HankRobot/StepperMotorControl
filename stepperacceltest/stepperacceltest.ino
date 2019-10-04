#include <AccelStepper.h>

AccelStepper myStepper1(1, 2, 3); //Platform Stepper, 2 pin -> step pin , 3 pin -> dir pin

void setup(){
    myStepper1.setMaxSpeed(1000);
}

void loop(){
    // Set the current position to 0:
    myStepper1.setCurrentPosition(0);
    // Run the motor forward at 200 steps/second until the motor reaches 400 steps (2 revolutions):
    while(myStepper1.currentPosition() != 400)
    {
        myStepper1.setSpeed(200);
        myStepper1.runSpeed();
    }
    delay(1000);
    // Reset the position to 0:
    myStepper1.setCurrentPosition(0);
    // Run the motor backwards at 600 steps/second until the motor reaches -200 steps (1 revolution):
    while(myStepper1.currentPosition() != -200) 
    {
        myStepper1.setSpeed(-600);
        myStepper1.runSpeed();
    }
    delay(1000);
    // Reset the position to 0:
    myStepper1.setCurrentPosition(0);
    // Run the motor forward at 400 steps/second until the motor reaches 600 steps (3 revolutions):
    while(myStepper1.currentPosition() != 600)
    {
        myStepper1.setSpeed(400);
        myStepper1.runSpeed();
    }
    delay(3000);
}