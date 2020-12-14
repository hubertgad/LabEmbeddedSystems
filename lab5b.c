typedef enum { fastLeft, left, stop, right, fastRight } state;
state currentState = stop;
bool reachedLeftLimit = false;
bool reachedRightLimit = false;

void setup()
{
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop()
{
  readButtonsInput();
  readLimitSwitches();
  moveMotor();
}

void readButtonsInput()
{
	switch (analogRead(A0) / 128)
    {
      case 0:
      {
        currentState = fastLeft;
        break;
      }
      case 1:
      {
        currentState = left;
        break;
      }
      case 2:
      {
        currentState = stop;
        break;
      }
      case 3:
      {
        currentState = right;
        break;
      }
      case 4:
      {
        currentState = fastRight;
        break;
      }
    }
}

void readLimitSwitches()
{
  if (!digitalRead(9) && (currentState == 0 || currentState == 1))
  {
	currentState = stop;
  }
  if (!digitalRead(10) && (currentState == 3 || currentState == 4))
  {
    currentState = stop;
  }
}

void moveMotor()
{
  switch (currentState)
  {
    case 0:
    {
      setSpeed(255);
      moveLeft();
      break;
    }
    case 1:
    {
      setSpeed(128);
      moveLeft();
      break;
    }
    case 2:
    {
      stopMoving();
      break;
    }
    case 3:
    {
      setSpeed(128);
      moveRight();
      break;
    }
    case 4:
    {
      setSpeed(255);
      moveRight();
      break;
    }
    default: break;
  }
}

void moveLeft()
{
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
}

void moveRight()
{
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
}

void stopMoving()
{
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);  
  setSpeed(0);
}

void setSpeed(int speed)
{
  analogWrite(3, speed);
}