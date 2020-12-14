#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h>
#include <string.h>

LiquidCrystal lcd(13, 12, 11, 10, 1, 0);

char keys[4][4] = {
	{'1','2','3', 'A'},
	{'4','5','6', 'B'},
	{'7','8','9', 'C'},
	{'*','0','#', 'D'}
};
byte rowPins[4] = {9, 8, 7, 6};
byte colPins[4] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4); 

int const pinLength = 6 + 1;

char currentPin[pinLength];
char validPin[pinLength] = {'1', '2', '3', '4', '\0'};
char tempPin[pinLength];

typedef enum { Locked, Unlocked, ChangePin } mode;
mode currentMode = Locked;

void setup()
{  
	initializeLcd();
	pinMode(A4, OUTPUT);
  	pinMode(A5, OUTPUT);
}

void loop()
{
	char key = keypad.getKey();

    switch (currentMode)
    {
		case Locked:
		{
			digitalWrite(A4, HIGH);
			digitalWrite(A5, LOW);
          
			manageLocked(key);
			break;
		}
		case Unlocked:
		{
          	digitalWrite(A4, LOW);
          	digitalWrite(A5, HIGH);
          
			manageUnlocked(key);
			break;
		}
		case ChangePin:
		{
          	digitalWrite(A4, LOW);
          	digitalWrite(A5, HIGH);
          
			manageChangePin(key);
			break;
		}      
    }
}

void initializeLcd()
{
	lcd.begin(16, 2);
	lcd.print("LOCKED");
  
  	lcd.setCursor(0, 1);
	lcd.print("PIN:");
}

void manageLocked(char key)
{
	bool isOver = insertPin(key, currentPin);
	
	if (isOver == true)
    {
		checkIfCorrect();
    }
}

void manageUnlocked(char key)
{
	if (key == '*' || key == '#')
	{
		currentMode = Locked;
		initializeLcd();
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
}

void manageChangePin(char key)
{
	bool isOver = insertPin(key, tempPin);
	
	if (isOver == true)
    {
		strcpy(validPin, tempPin);
		removeLastChar(tempPin);
      
		lcd.clear();
      
      	lcd.setCursor(0, 0);      
      	lcd.print("PIN SAVED");
      
		currentMode = Locked;
      
		reinitializeLcd();
    }
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

void checkIfCorrect()
{
  	lcd.clear();
	lcd.setCursor(0, 0);
  
	if (strncmp(currentPin, validPin, pinLength - 1) == 0)
    {
		lcd.print("ACCESS GRANTED");
      	lcd.setCursor(0, 1);
      
		lcd.print("A - CHANGE PIN");
		currentMode = Unlocked;
    }
  	else
    {
		lcd.print("ACCESS DENIED");
		reinitializeLcd();
    }
  
	for (int i = 0; i < pinLength; i++)
    {
		currentPin[i] = NULL;
    }
}

void reinitializeLcd()
{
    lcd.setCursor(0, 1);
	lcd.print("WAIT");
  
	lcd.setCursor(5, 1);
	for (int i = 3; i > 0; i--)
	{
		lcd.setCursor(5, 1);
		lcd.print(i);
		delay(1000);
	}
  
	initializeLcd();
}