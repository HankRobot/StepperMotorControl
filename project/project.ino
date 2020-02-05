#include <AccelStepper.h> //Stepper Motor Library http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#ace236ede35f87c63d18da25810ec9736

//----------------------------------------------------Stepper Motor Variables-------------------------------------------------------------
//PIN SETTINGS
AccelStepper myStepper1(1, 2, 3); //Platform Stepper
AccelStepper myStepper2(1, 4, 5); //Platform Stepper
AccelStepper myStepper3(1, 6, 7); //Conveyor Belt Stepper

//STEPPER MOTOR SPEEDS
int s = 2000; //Platform Motor Speed
int c = -115;  //Conveyor Belt Motor Speed
int startspeed = 3500; //Positioning Speed
int c_fill = -300; //Speed to fill fibre on conveyor belt

//NUMBER OF STEPS
//3200 steps for 1 full revolution
//1 rev == 3.7cm (for platform)
//1 rev == 7.8cm (for conveyor belt)
int F_start = 3200 * 9.9; //FORWARD
int F = 3200 * 6.2; //FORWARD
int B = 3200 * 6.2; //BACKWARD
int L = 3200 * 2.0; //LEFT
int R = 3200 * 2.0; //RIGHT
int conveyor_mv = 3200 * 3.42; //CONVEYOR BELT TO DEPOSIT PLATFORM WITH FIBRE

//-----------------------------------------------------------Buttons----------------------------------------------------------------- 
const int conv_butt = 8; //Button for Conveyor Belt
const int plat_butt = 9; //Button for Platform
const int x_butt = 10; //Horizontal button for Positioning at the Start
const int y_butt = 11; //Vertical button for Positioning at the Start

//DIGITAL MODES
bool pONOFF = false;     //Digital Mode for Platform Button
bool cONOFF = false;     //Digital Mode for Conveyor Button
bool xSTART = false;     //Digital Mode for Horizontal Positioning of Platform
bool ySTART = false;     //Digital Mode for Vertical Positioning of Platform

//ARBITRARY INCREMENTS (to set conditions)
int state = 0;        //Increment for Platform Movement DURING RUN
int countSTART = 0;   //Increment for Platform Movement WHEN REPOSITIONING
int nick = 1;         //Increment for FRONT TO LEFT Movement

//----------------------------------------------------STEPPER MOTOR FUNCTIONS---------------------------------------------------------
//to stop all motors
void allStop() {
  /*
  PORTD = B00000000; //sets all of the pins 0 to 7 as LOW to power off stepper1
  PORTB = B00000000; //sets all of the pins 8 to 13 as LOW to power off stepper2
  */
  for (int i = 2; i < 14; i++)
  {
    digitalWrite(i,LOW); //same method, but for pin 2 to 13 as it doesnt affect TX RX
  }
}

//to move backwards when positioning platform at the start
void backSTART() {
  myStepper1.setSpeed(-startspeed);
  myStepper2.setSpeed(startspeed);
  myStepper1.runSpeed();
  myStepper2.runSpeed();
}

//to move backwards when positioning platform at the start
void rightSTART() {
  myStepper1.setSpeed(startspeed);
  myStepper2.setSpeed(startspeed);
  myStepper1.runSpeed();
  myStepper2.runSpeed();
}

//put fibre on conveyor belt
void beltFill(int c_fill) {
  myStepper3.setSpeed(c_fill);
  myStepper3.runSpeed();
}

//to move conveyor belt 
void beltStep(int c) {
  myStepper3.moveTo(conveyor_mv);
  myStepper3.setSpeed(c);
  myStepper3.run();
}

//to move the motors forward
void forwardStep(int F, int s) {
  myStepper1.moveTo(F);
  myStepper2.moveTo(-F);
  myStepper1.setSpeed(s);
  myStepper2.setSpeed(-s);
  myStepper1.run();
  myStepper2.run();
}

// to move the motors back
void backwardStep(int B, int s) {
  myStepper1.moveTo(-B);
  myStepper2.moveTo(B);
  myStepper1.setSpeed(-s);
  myStepper2.setSpeed(s);
  myStepper1.run();
  myStepper2.run();
}

// to move the motors in opposite directions (left)
void leftStep(int L, int s) {
  myStepper1.moveTo(-L);
  myStepper2.moveTo(-L);
  myStepper1.setSpeed(-s);
  myStepper2.setSpeed(-s);
  myStepper1.run();
  myStepper2.run();
}

// to move the motors in opposite directions (right)
void rightStep(int R, int s) {
  myStepper1.moveTo(R);
  myStepper2.moveTo(R);
  myStepper1.setSpeed(s);
  myStepper2.setSpeed(s);
  myStepper1.run();
  myStepper2.run();
}

//------------------------------------------------------------Void Setup--------------------------------------------------------------------------------

//run functions once
void setup() {
  //To setup Serial Monitor
  Serial.begin(9600);
  //Setup pins for buttons
  pinMode (8, INPUT);
  pinMode (9, INPUT);
  pinMode (10, INPUT);
  pinMode (11, INPUT);
  pinMode (13, OUTPUT);
  //Setting Max Speed of Stepper Motors
  myStepper1.setMaxSpeed(4000);
  myStepper2.setMaxSpeed(4000);
  myStepper3.setMaxSpeed(c);
  myStepper1.setCurrentPosition(0);
  myStepper2.setCurrentPosition(0);
  myStepper3.setCurrentPosition(0);

  while (countSTART == 0) {
    //Serial.println(digitalRead(x_butt));
    //CALLING BUTTON LOGICS
    if (digitalRead(x_butt) == LOW) {
      xSTART = true;
    }
    if (digitalRead(y_butt) == LOW) {
      ySTART = true;  
    }
  
    while (ySTART == false) {
      backSTART();   //move downwards to hit bottom button     
      break;
    }

    while (xSTART == false) {
      rightSTART();    //move right to hit right button
      break;
    }

    while (xSTART == true && ySTART == true) {
      myStepper1.setCurrentPosition(0);
      myStepper2.setCurrentPosition(0);
      allStop();
      countSTART ++;
      break;
    }
    
  }
  
}

//------------------------------------------------------------Void Loop--------------------------------------------------------------------------------

//run functions in a loop
void loop() {
  if (digitalRead(conv_butt) == LOW) {  
    cONOFF = true;
  }
  
  if ( digitalRead(plat_butt) == LOW) {
    pONOFF = true;
  }

  //IF BOTH BUTTONS ARE PRESSED
  if (digitalRead(conv_butt) == LOW && digitalRead(plat_butt) == LOW) {
    allStop();
    cONOFF = false;
    pONOFF = false;
  }

  //IF PLATFORM BUTTON IS PRESSED (PRESS 7 TIMES TO COMPLETE CYCLE)
  if (pONOFF == true && state == 0) {              //MOVE FORWARDS
    myStepper3.disableOutputs();
    forwardStep(F_start, startspeed);
    if (myStepper1.distanceToGo() == 0) {
      myStepper1.setCurrentPosition(0);
      myStepper2.setCurrentPosition(0);
      state ++;
      pONOFF = false;
      Serial.println("State 1 done!");
    }
  } 
  
  else if (pONOFF == true && state == 1) {         //MOVE BACKWARDS
    myStepper3.enableOutputs();
    backwardStep(B, s);
    beltStep(c);
    if (myStepper1.distanceToGo() == 0) {
      myStepper1.setCurrentPosition(0);
      myStepper2.setCurrentPosition(0);
      state ++;
      pONOFF = false;
    }
  }

  else if (pONOFF == true && state == 2) {    //MOVE FORWARD
    if (nick == 1) {
      myStepper3.disableOutputs();
      forwardStep(F, s);
      //beltStep(c);
      if (myStepper1.distanceToGo() == 0) {
        myStepper1.setCurrentPosition(0);
        myStepper2.setCurrentPosition(0);
        nick = 2;
      }
    }
    else if (nick == 2) {
      leftStep(L, s);
      if (myStepper1.distanceToGo() == 0) {   //MOVE LEFT
        myStepper1.setCurrentPosition(0);
        myStepper2.setCurrentPosition(0);
        state = 3;
        nick = 3;
        pONOFF = false;
      }
    }
  }
  

  else if (pONOFF == true && state == 3) {    //MOVE BACKWARD
    myStepper3.enableOutputs();
    backwardStep(B, s);
    beltStep(c);
    if (myStepper1.distanceToGo() == 0) {
      myStepper1.setCurrentPosition(0);
      myStepper2.setCurrentPosition(0);
      state ++;
      pONOFF = false;
    }
  }

  else if (pONOFF == true && state == 4) {    //MOVE FORWARD
    if (nick == 3) {
      myStepper3.disableOutputs();
      forwardStep(F, s);
      //beltStep(c);
      if (myStepper1.distanceToGo() == 0) {
        myStepper1.setCurrentPosition(0);
        myStepper2.setCurrentPosition(0);
        nick = 4;
      }
    }
    else if (nick == 4) {                     //MOVE LEFT
      leftStep(L, s);
      if (myStepper1.distanceToGo() == 0) {
        myStepper1.setCurrentPosition(0);
        myStepper2.setCurrentPosition(0);
        state ++;   
        pONOFF = false;
      }
    }
  }

  else if (pONOFF == true && state == 5) {    //MOVE BACKWARD
    myStepper3.enableOutputs();
    backwardStep(F_start, s);
    beltStep(c);
    if (myStepper1.distanceToGo() == 0) {
      myStepper1.setCurrentPosition(0);
      myStepper2.setCurrentPosition(0);
      state ++;
      pONOFF = false;
    }
  }
  /*
  //MOVE CONVEYOR BELT WHEN PRESSED
  if (cONOFF == true) {
    digitalWrite(13,HIGH); 
    beltFill(c_fill);
    //cONOFF = false;
  }
  else
  {
    digitalWrite(13,LOW);
  }
  */
}
