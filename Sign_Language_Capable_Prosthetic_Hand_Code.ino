#include "HCPCA9685.h" //Provided library for PWM board.

/* I2C slave address for the device/module. For the HCMODU0097 the default I2C address
   is 0x40 */
#define  I2CAdd 0x40

//Definitions for Full, Half, and Open fingers, and Left, Right, and Neutral side-to-side movement
#define Full 1.0
#define FullBase 0.5
#define Half 0.85
#define ThumbHalf 0.5
#define HalfBase 0.4
#define Open 0.0
#define Neutral 0.5
#define Left 1.0
#define Right 0.0

//Definitions to hold the servo number for each finger artiulation
#define ThumbFlex 0
#define IndexFlex 1
#define MiddleFlex 2
#define RingFlex 3
#define PinkyFlex 4

#define IndexBase 5
#define MiddleBase 6

#define IndexSide 7
#define RingSide 8
#define PinkySide 9

#define ThumbRotate 10
#define ThumbSide 11

//Array to store the last position of the servos, for Move()
float lastPos[12] = {0.0};

//Flag to change the hand from signing in BSL to ASL
int BSL = 0;

HCPCA9685 HCPCA9685(I2CAdd); //Create an instance of the library.

//Define a structure to hold the endstop values for the servos
typedef struct
{
  int minVal;
  int maxVal;
} servoMinMax;

//Endstop values for the servos
servoMinMax motors[12] = {
  {560, 220}, //ThumbFlex
  {600, 175}, //IndexFlex
  {70, 350},  //MiddleFlex
  {110, 480}, //RingFlex
  {110, 480}, //PinkyFlex
  {490, 110}, //IndexBase
  {110, 550}, //MiddleBase
  {250, 350}, //IndexSide
  {275, 325}, //RingSide
  {230, 280}, //PinkySide
  {550, 350}, //ThumbRotate
  {590, 400}  //ThumbSide
};

void setup() {
  // Initialise the library in 'servo mode' and wake the device up.
  HCPCA9685.Init(SERVO_MODE);
  
  HCPCA9685.Sleep(false);

}

void loop() {
  // put your main code here, to run repeatedly:
  
    OpenHand();
    T();

}

//Function to normalise the servo positions to between 0.0 and 1.0, for ease of setting
void servoMove(unsigned int servoNum, float servoPos)
{
  int c = motors[servoNum].minVal;
  float m = motors[servoNum].maxVal - motors[servoNum].minVal;

  HCPCA9685.Servo(servoNum, (c +  (int)(m * servoPos)));
}

//Function to move the servos
void Move(unsigned int servoNum, float servoPos)
{
  //Float to hold the position value sent to the servo
  float Pos;
  //Sweep the servo from its last position to the specified end position
  //If the specified end position is greater than the last servo position
  if (servoPos > lastPos[servoNum]) {
    //Add 0.01 to the last position until the end position is reached
    for (Pos = lastPos[servoNum]; Pos <= servoPos; Pos += 0.01)
    {
      servoMove(servoNum, Pos);
      delay(10);
    }
  //If the specified end position is less than the last servo position
  } else {
    //Subtract 0.01 from the last position until the end position is reached
    for (Pos = lastPos[servoNum]; Pos >= servoPos; Pos -= 0.01)
    {
      servoMove(servoNum, Pos);
      delay(10);
    }
  }
  lastPos[servoNum] = servoPos;
  //Wait for 1s
  delay(1000);
}

void Point() {
  //Servo positions to make the hand point
  Move(ThumbRotate, ThumbHalf);
  Move(ThumbFlex, Full);
  Move(IndexFlex, Open);
  Move(MiddleFlex, Full);
  Move(RingFlex, Full);
  Move(PinkyFlex, Full);
}

void PinkyPoint() {
  //Servo positions to make the hand have only the pinky extended
  Move(ThumbRotate, ThumbHalf);
  Move(ThumbFlex, Full);
  Move(IndexFlex, Full);
  Move(MiddleFlex, Full);
  Move(RingFlex, Full);
  Move(PinkyFlex, Open);
}

void Fist() {
  //Servo positions to make the hand form a fist
  Move(ThumbRotate, ThumbHalf);
  Move(ThumbFlex, Full);
  Move(IndexFlex, Full);
  Move(MiddleFlex, Full);
  Move(RingFlex, Full);
  Move(PinkyFlex, Full);
}

void OpenHand() {
  //Return all servos to their rest positions
  Move(ThumbFlex, Open);
  Move(ThumbSide, Open);
  Move(ThumbRotate, Open);
  Move(IndexFlex, Open);
  Move(IndexBase, Open);
  Move(IndexSide, Neutral);
  Move(MiddleFlex, Open);
  Move(MiddleBase, Open);
  Move(RingFlex, Open);
  Move(RingSide, Right);
  Move(PinkyFlex, Open);
  Move(PinkySide, Right);
  delay(1000);
}

//Letter functions for both BSL and ASL

void A() {
  if(BSL == 1){
    //BSL sign
    Point();
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(IndexFlex, Full);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    delay(2000);
    OpenHand();
  }
}

void B() {
  if(BSL == 1){
    //BSL sign
    Move(IndexFlex, Half);
    Move(MiddleFlex, Half);
    Move(RingFlex, Half);
    Move(PinkyFlex, Half);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbSide, Half);
    Move(ThumbFlex, Half);
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(IndexFlex, Open);
    Move(MiddleFlex, Open);
    Move(RingFlex, Open);
    Move(PinkyFlex, Open);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Half);
    delay(2000);
    OpenHand();
  }
}

void C() {
  if(BSL == 1){
    //BSL sign
    Move(IndexFlex, Half);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbSide, Half);
    Move(ThumbFlex, Half);
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(IndexFlex, Half);
    Move(MiddleFlex, Half);
    Move(RingFlex, Half);
    Move(PinkyFlex, Half);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbSide, Half);
    Move(ThumbFlex, Half);
    delay(2000);
    OpenHand();
  }
}

void D() {
  if(BSL == 1){
    //BSL sign
    Move(IndexFlex, Half);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbSide, Half);
    Move(ThumbFlex, Half);
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(IndexFlex, Open);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Half);
    delay(2000);
    OpenHand();
  }
}

void E() {
  if(BSL == 1){
    //BSL sign
    Point();
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    Move(IndexFlex, Full);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    delay(2000);
    OpenHand();
  }
}

void FLetter() {
  if(BSL == 1){
    //BSL sign
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    Move(IndexFlex, Open);
    Move(MiddleFlex, Open);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(IndexFlex, Half);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbSide, ThumbHalf);
    Move(ThumbFlex, Half);
    delay(2000);
    OpenHand();
  }
}

void G() {
  if(BSL == 1){
    //BSL sign
    Fist();
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Point();
    delay(2000);
    OpenHand();
  }
}

void H() {
  if(BSL == 1){
    //BSL sign
    OpenHand();
  } else {
    //ASL sign
    Move(IndexFlex, Open);
    Move(MiddleFlex, Open);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    delay(2000);
    OpenHand();
  }
}

void I() {
  if(BSL == 1){
    //BSL sign
    Point();
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    PinkyPoint();
    delay(2000);
    OpenHand();
  }
}

void J() {
  if(BSL == 1){
    //BSL sign
    Point();
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    PinkyPoint();
    //Wrist move
    delay(2000);
    OpenHand();
  }
}

void K() {
  if(BSL == 1){
    //BSL sign
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    Move(IndexFlex, Half);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(ThumbRotate, Full);
    Move(ThumbFlex, Open);
    Move(IndexFlex, Open);
    Move(IndexSide, Right);
    Move(MiddleFlex, Open);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    delay(2000);
    OpenHand();
  }
}

void L() {
  if(BSL == 1){
    //BSL sign
    Point();
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(ThumbRotate, Open);
    Move(ThumbSide, Full);
    Move(IndexFlex, Open);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    delay(2000);
    OpenHand();
  }
}

void M() {
  if(BSL == 1){
    //BSL sign
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    Move(IndexFlex, Open);
    Move(MiddleFlex, Open);
    Move(RingFlex, Open);
    Move(PinkyFlex, Full);
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    Move(IndexFlex, Full);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    delay(2000);
    OpenHand();
  }
}

void N() {
  if(BSL == 1){
    //BSL sign
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    Move(IndexFlex, Open);
    Move(MiddleFlex, Open);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(PinkyFlex, Full);
    Move(RingFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    Move(IndexFlex, Full);
    Move(MiddleFlex, Full);
    delay(2000);
    OpenHand();
  }
}

void O() {
  if(BSL == 1){
    //BSL sign
    Point();
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(IndexFlex, Half);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbSide, ThumbHalf);
    Move(ThumbFlex, Half);
    delay(2000);
    OpenHand();
  }
}

void P() {
  if(BSL == 1){
    //BSL sign
    Move(IndexFlex, Half);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbSide, ThumbHalf);
    Move(ThumbFlex, Half);
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(IndexFlex, Open);
    Move(MiddleBase, FullBase);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbSide, ThumbHalf);
    Move(ThumbFlex, Open);
    delay(2000);
    OpenHand();
  }
}

void Q() {
  if(BSL == 1){
    //BSL sign
    Point();
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(IndexBase, FullBase);
    Move(MiddleFlex, Open);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbSide, ThumbHalf);
    Move(ThumbFlex, Open);
    delay(2000);
    OpenHand();
  }
}

void R() {
  if(BSL == 1){
    //BSL sign
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    Move(IndexFlex, Half);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    delay(2000);
    OpenHand();
  } else {
      //ASL sign
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    Move(IndexBase, HalfBase);
    Move(MiddleFlex, Open);
    Move(IndexSide, Left);
    delay(2000);
    Move(IndexSide, Neutral);
    Move(IndexBase, Open);
    OpenHand();
  }
}

void S() {
  if(BSL == 1){
    //BSL sign
    PinkyPoint();
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(IndexFlex, Full);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbSide, ThumbHalf);
    Move(ThumbFlex, Half);
    delay(2000);
    OpenHand();
  }
}

void T() {
  if(BSL == 1){
    //BSL sign
    Point();
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(PinkyFlex, Full);
    Move(RingFlex, Full);
    Move(MiddleFlex, Full);
    Move(ThumbRotate, Full);
    Move(ThumbFlex, Open);
    Move(IndexFlex, Full);
    delay(2000);
    OpenHand();
  }
}

void U() {
  if(BSL == 1){
    //BSL sign
    Point();
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(IndexFlex, Open);
    Move(MiddleFlex, Open);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    delay(2000);
    OpenHand();
  }
}

void V() {
  if(BSL == 1){
    //BSL sign
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    Move(IndexFlex, Open);
    Move(IndexSide, Right);
    Move(MiddleFlex, Open);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    Move(IndexFlex, Open);
    Move(IndexSide, Right);
    Move(MiddleFlex, Open);
    delay(2000);
    OpenHand();
  }
}

void W() {
  if ((BSL == 1)) {
    //BSL sign
    Move(ThumbRotate, Open);
    Move(ThumbSide, Half);
    Move(ThumbFlex, Open);
    Move(IndexFlex, Open);
    Move(IndexSide, Right);
    Move(MiddleFlex, Open);
    Move(RingFlex, Open);
    Move(PinkyFlex, Open);
    Move(PinkySide, Left);
    Move(RingSide, Left);
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(PinkyFlex, Full);
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    Move(IndexFlex, Open);
    Move(IndexSide, Right);
    Move(MiddleFlex, Open);
    Move(RingFlex, Open);
    Move(RingSide, Left);
    delay(2000);
    OpenHand();
  }
}

void X() {
  if (BSL == 1) {
    //BSL sign
    Point();
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(ThumbRotate, ThumbHalf);
    Move(ThumbFlex, Full);
    Move(IndexFlex, Half);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Full);
    delay(2000);
    OpenHand();
  }
}

void Y() {
  if (BSL == 1) {
    //BSL sign
    Point();
    delay(2000);
    OpenHand();
  } else {
    //ASL sign
    Move(ThumbSide, ThumbHalf);
    Move(ThumbFlex, Open);
    Move(IndexFlex, Full);
    Move(MiddleFlex, Full);
    Move(RingFlex, Full);
    Move(PinkyFlex, Open);
    Move(PinkySide, Left);
    delay(2000);
    OpenHand();
  }
}

void Z() {
  if (BSL == 1) {
    //BSL sign
    OpenHand();
  } else {
    //ASL sign
    Point();
    delay(2000);
    OpenHand();
  }
}
