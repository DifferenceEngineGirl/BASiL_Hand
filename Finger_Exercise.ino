/* Servo library created by: Andrew Davies
   Code based off a Sketch created by Hobby Components Ltd (HOBBYCOMPONENTS.COM)*/

/* Include the HCPCA9685 library */
#include "HCPCA9685.h"

/* I2C slave address for the device/module. For the HCMODU0097 the default I2C address
   is 0x40 */
#define  I2CAdd 0x40

//Definitions for Full, Half, and Open fingers, and Left, Right, and Neutral side-to-side movement
#define Full 1.0
#define Half 0.85
#define ThumbHalf 0.5
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

/* Create an instance of the library */
HCPCA9685 HCPCA9685(I2CAdd);

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
  if (servoPos > lastPos[servoNum]) {
    for (Pos = lastPos[servoNum]; Pos <= servoPos; Pos += 0.01)
    {
      servoMove(servoNum, Pos);
      delay(10);
    }
  } else {
    for (Pos = lastPos[servoNum]; Pos >= servoPos; Pos -= 0.01)
    {
      servoMove(servoNum, Pos);
      delay(10);
    }
  }
  lastPos[servoNum] = servoPos;
  delay(1000);
}

void setup()
{
  /* Initialise the library and set it to 'servo mode' */
  HCPCA9685.Init(SERVO_MODE);

  /* Wake the device up */
  HCPCA9685.Sleep(false);
  
}


void loop()
{

  //Ensure all motors are in their rest position
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

  //Move single joint bend
  Move(MiddleBase, ThumbHalf);
  Move(MiddleBase, Open);
  delay(25);
  Move(IndexBase, ThumbHalf);
  Move(IndexBase, Open);
  delay(25);
  Move(MiddleBase, ThumbHalf);
  Move(IndexBase, ThumbHalf);
  delay(50);
  Move(MiddleBase, Open);
  Move(IndexBase, Open);
  delay(25);

  //Cross fingers
  Move(ThumbRotate, ThumbHalf);
  Move(ThumbFlex, Full);
  Move(PinkyFlex, Full);
  Move(RingFlex, Full);
  Move(IndexBase, 0.4);
  Move(IndexSide, Left);
  delay(1000);
  Move(IndexSide, Neutral);
  Move(IndexBase, Open);
  delay(25);
  Move(PinkyFlex, Open);
  Move(RingFlex, Open);
  Move(ThumbFlex, Full);
  Move(ThumbRotate, ThumbHalf);

  //Move fingers individually
  Move(PinkyFlex, Full);
  Move(PinkyFlex, Open);
  delay(25);
  Move(RingFlex, Full);
  Move(RingFlex, Open);
  delay(25);
  Move(MiddleFlex, Full);
  Move(MiddleFlex, Open);
  delay(25);
  Move(IndexFlex, Full);
  Move(IndexFlex, Open);
  delay(25);

  //Move fingers together
  Move(PinkyFlex, Full);
  Move(RingFlex, Full);
  Move(MiddleFlex, Full);
  Move(IndexFlex, 0.95);
  delay(50);
  Move(PinkyFlex, Open);
  Move(RingFlex, Open);
  Move(MiddleFlex, Open);
  Move(IndexFlex, Open);

  //Move sides
  Move(PinkySide, Left);
  delay(25);
  Move(RingSide, Left);
  delay(25);
  Move(IndexSide, Right);
  delay(50);
  Move(RingSide, Right);
  delay(25);
  Move(PinkySide, Right);
  delay(25);
  Move(IndexSide, Neutral);
  delay(25);

  //Move thumb
  Move(ThumbRotate, Full);
  Move(ThumbRotate, Open);
  delay(25);
  Move(ThumbSide, Full);
  Move(ThumbSide, Open);
  delay(25);
  Move(ThumbRotate, ThumbHalf);
  delay(15);
  Move(ThumbFlex, Full);
  Move(ThumbFlex, Open);
  delay(25);
  Move(ThumbRotate, Open);

  //Rock on
  Move(ThumbRotate, ThumbHalf);
  Move(ThumbFlex, Full);
  Move(RingFlex, Full);
  Move(MiddleFlex, Full);
  delay(50);
  Move(RingFlex, Open);
  Move(MiddleFlex, Open);
  Move(ThumbFlex, Open);
  Move(ThumbRotate, Open);
}
