int motionSensorInput = 0;
int tmpSensorInput = 0;
int tmp255 = 0;
int green = 0;
int blue = 0;

void setup()
{ 
  pinMode(11, INPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
}

void loop()
{
  motionSensorInput = digitalRead(11);
  tmpSensorInput = analogRead(A0);
  
  if (motionSensorInput == HIGH) 
  {
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    delay(150);
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    delay(150);
  }
  else
  {
    digitalWrite(13, LOW);
	digitalWrite(12, LOW);
  }
  
  tmp255 = (tmpSensorInput - 20) * 0.7544379;
  blue = 255 - tmp255;
  green = 255 - abs(127 - tmp255) * 2 - 1;
  
  analogWrite(A1, tmp255);
  analogWrite(A2, blue);
  analogWrite(A3, green);
}