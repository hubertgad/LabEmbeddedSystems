#include <Servo.h>

Servo blueOne;
Servo blackOne;

int bluePosition = 90;
int blackPosition = 90;

typedef enum { left, right, stop } direction;
direction blueDirection = stop;

void setup()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  blueOne.attach(9, 500, 2500);
  blackOne.attach(8, 500, 2500);
}

void loop()
{
  readInputs();
  moveBlueOne();
  moveBlackOne();  
  delay(50);
}

void readInputs()
{
  switch (analogRead(A0) / 128)
  {
    case 0:
    {
      blueDirection = left;
      break;
    }
    case 1:
    {
      blueDirection = right;
      break;
    }
    default:
    {
      blueDirection = stop;
      break;
    }
  }  
  blackPosition = 180 - (analogRead(A1)* 18 / 102);
}

void moveBlueOne()
{
  switch (blueDirection)
  {
    case left:
    {
      if (bluePosition > 0) 
        blueOne.write(--bluePosition);
      break;
    }
    case right:
    {
      if (bluePosition < 180) 
        blueOne.write(++bluePosition);
      break;
    }
    default: break;
  }
}

void moveBlackOne()
{
  blackOne.write(blackPosition);
}