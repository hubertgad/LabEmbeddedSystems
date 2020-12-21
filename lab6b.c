#include <LiquidCrystal.h>
#include <Key.h>
#include <IRremote.h>
#include <string.h>

LiquidCrystal lcd(13, 12, 11, 10, 1, 0);

IRrecv irrecv(A3);
decode_results results;

int const pinLength = 6 + 1;

char currentPin[pinLength];
char validPin[pinLength] = {'1', '2', '3', '4', '\0'};
char tempPin[pinLength];

typedef enum { Unlocked, Locked, MotionDetected, Alarm, ChangePin } mode;
mode currentMode = Locked;

void setup()
{  
	lcd.begin(16, 2);
	enterLockedMode("Hello!");
    irrecv.enableIRIn();
  	pinMode(2, INPUT);
  	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
}

char key;
unsigned long motionTime = 0;

void loop()
{
	getKey();
  
    switch (currentMode)
    {
		case Unlocked:
			manageUnlockedMode(key);
			break;
		case Locked:
			manageLockedMode(key);
			break;
      	case MotionDetected:
      		manageMotionDetectedMode(key);
      		break;
      	case Alarm:
      		manageAlarmMode(key);
      		break;
		case ChangePin:
			manageChangePinMode(key);
			break;
    }
}

void getKey()
{
	key = NO_KEY;
  	
  	if (irrecv.decode(&results))
    {
      switch(results.value)
      {
        case 0xFD10EF:
        	key = '*';
        	break;
        case 0xFD50AF:
        	key = '#';
        	break;
        case 0xFDB04F:
        	key = 'A';
        	break;
		case 0xFD30CF:
        	key = '0';
        	break;
		case 0xFD08F7:
			key = '1';
			break;
		case 0xFD8877:
			key = '2';
			break;
		case 0xFD48B7:
			key = '3';
			break;
		case 0xFD28D7:
			key = '4';
			break;
		case 0xFDA857:
			key = '5';
			break;
		case 0xFD6897:
			key = '6';
			break;
		case 0xFD18E7:
			key = '7';
			break;
		case 0xFD9867:
			key = '8';
			break;
		case 0xFD58A7:
			key = '9';
			break;
        default: break;
      }
      irrecv.resume();
    }
}

void enterUnlockedMode()
{
	currentMode = Unlocked;
  	prepareMode("Access granted.", "EQ - CHANGE PIN");
}

void enterLockedMode(char msg[])
{
  	currentMode = Locked;
	lcd.clear();
	lcd.setCursor(0, 0);      
    lcd.print(msg);
    lcd.setCursor(0, 1);
	lcd.print("WAIT");
  
	lcd.setCursor(5, 1);
	for (int i = 3; i > 0; i--)
	{
		lcd.setCursor(5, 1);
		lcd.print(i);
		delay(1000);
	}
  
  	prepareMode("LOCKED", "PIN: ");
}

void enterMotionDetectedMode()
{
	currentMode = MotionDetected;
	prepareMode("I SEE YOU", "PIN: ");
	motionTime = millis() / 1000;
}

void enterAlarmMode()
{
	currentMode = Alarm;
	prepareMode("INVASION ALARM!", "PIN: ");
}

void enterChangePinMode()
{
	currentMode = ChangePin; 
	prepareMode("Change PIN", "NEW:");
}

void prepareMode(char firstLine[], char secondLine[])
{
	lcd.clear();
	lcd.print(firstLine);
	lcd.setCursor(0, 1);
	lcd.print(secondLine);
}

void manageUnlockedMode(char key)
{
	if (key == '*' || key == '#')
	{
		enterLockedMode("LOCKING...");
    }
  	else if (key == 'A')
    {
		lcd.clear();
      
        lcd.setCursor(0, 0);
        lcd.print("CHANGE PIN");
      
        lcd.setCursor(0, 1);
        lcd.print("NEW: ");
      
        lcd.setCursor(5, 1);
		currentMode = ChangePin;
    }
  	digitalWrite(3, HIGH);	
	digitalWrite(4, LOW);
	digitalWrite(5, LOW);
	digitalWrite(6, LOW);
}

void manageLockedMode(char key)
{
  	if (digitalRead(2)) 
    {
		enterMotionDetectedMode();
    }
  
	bool isOver = insertPin(key, currentPin);
	
	if (isOver == true)
    {
		
		if (checkIfCorrect())
		{
			enterUnlockedMode();
		}
		else
		{
			enterLockedMode("ACCESS DENIED");
	    }
    }
  	digitalWrite(3, LOW);	
	digitalWrite(4, LOW);
	blink(5);
	digitalWrite(6, LOW);
}

void manageMotionDetectedMode(char key)
{
	lcd.setCursor(10, 0);
  	if ((millis() / 1000 - motionTime) > 9)
    {
		enterAlarmMode();
    }
	lcd.print(9 - (millis() / 1000 - motionTime));
  	
  	bool isOver = insertPin(key, currentPin);
	
	if (isOver == true)
    {
		if (checkIfCorrect())
		{
			enterUnlockedMode();
		}
		else
		{
			enterAlarmMode();
	    }
    }
	digitalWrite(3, LOW);	
	digitalWrite(4, HIGH);
	blink(5);
	digitalWrite(6, LOW);
}

void manageAlarmMode(char key)
{
	bool isOver = insertPin(key, currentPin);
	
	if (isOver == true)
    {
		if (checkIfCorrect())
        {
			enterUnlockedMode();
        }
      	else
    	{
			enterAlarmMode();
    	}
    }
	digitalWrite(3, LOW);	
	digitalWrite(4, HIGH);
	blink(5);
	blink2(6);
}

void manageChangePinMode(char key)
{
	bool isOver = insertPin(key, tempPin);
	
	if (isOver == true)
    {
		strcpy(validPin, tempPin);
		removeLastChar(tempPin);
      
		lcd.clear();
      
		enterLockedMode("PIN SAVED");
    }
  	digitalWrite(3, HIGH);	
	blink(4);
	digitalWrite(5, LOW);
	digitalWrite(6, LOW);
}

bool insertPin(char key, char pin[])
{
	switch (key)
    {
		case NO_KEY: return false;
		case '*':
		{
			removeLastChar(pin);
			printPin(pin);
          
			return false;
		}
		case '#':
		{
			return true;
        }
		default:
		{
			insertChar(key, pin);
			printPin(pin);
          
			return false;
        }
    }
}

void insertChar(char c, char pin[])
{
	for (int i = 0; i < pinLength - 1; i++)
	{
		if (pin[i] == NULL)
        {
			pin[i] = c;
			pin[i + 1] = '\0';
			          
			break;
        }
	}
}

void removeLastChar(char pin[])
{
	for (int i = pinLength - 2; i >= 0; i--)
    {
      	if (pin[i] != NULL)
        { 
			pin[i] = NULL;
			return;
        }
    }
}

void printPin(char pin[])
{
  	lcd.setCursor(5, 1);

	for (int i = 0; i < pinLength - 1; i++)
    {
		if (pin[i] != NULL)
        {
			lcd.print('*');
        }
		else
		{
			lcd.print(' ');
		}      
    }
}

bool checkIfCorrect()
{
	bool isCorrect = false;
  
	if (strncmp(currentPin, validPin, pinLength - 1) == 0)
    {
      	isCorrect = true;
    }
  	else
    {
		isCorrect = false;
    }
  
	for (int i = 0; i < pinLength; i++)
    {
		currentPin[i] = NULL;
    }
  
	return isCorrect;
}

void blink(int pin)
{
	if (millis() % 400 < 200)
    {
      digitalWrite(pin, HIGH);
    }
  	else
    {
      digitalWrite(pin, LOW);
    }
}

void blink2(int pin)
{
	if (millis() % 400 > 200)
    {
      digitalWrite(pin, HIGH);
    }
  	else
    {
      digitalWrite(pin, LOW);
    }
}