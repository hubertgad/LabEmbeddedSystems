bool ledState[4] = {0, 0, 0, 0};
bool display1State[7] = {0, 0, 0, 0, 0, 0, 0};
bool display2State[7] = {0, 0, 0, 0, 0, 0, 0};
bool displayFirst = true;
int numberToDisplay = 0;

void setup()
{
	for (int i = 0; i <= 13; i++)
    {
		pinMode(i, OUTPUT);
    }
  
	pinMode(A5, INPUT_PULLUP);
	pinMode(A4, INPUT_PULLUP);
	pinMode(A3, INPUT_PULLUP);
	pinMode(A2, INPUT_PULLUP);
}

void loop()
{
	readSwitchesState();  
	controlLEDDiodes();  
	controlDigitalDisplays();
}

void readSwitchesState()
{
	ledState[0] = !digitalRead(A2);
	ledState[1] = !digitalRead(A3);
	ledState[2] = !digitalRead(A4);
	ledState[3] = !digitalRead(A5);
  
	numberToDisplay = 1 * ledState[0] + 2 * ledState[1] + 4 * ledState[2] + 8 * ledState[3];
}

void controlLEDDiodes()
{
	digitalWrite(13, ledState[0]);
	digitalWrite(12, ledState[1]);
	digitalWrite(11, ledState[2]);
	digitalWrite(10, ledState[3]);
}


void controlDigitalDisplays()
{
	assignNumberToDisplays();
  
	if (displayFirst == true)
	{
		digitalWrite(0, LOW);
		displayValues(display1State);
		digitalWrite(1, HIGH);
		displayFirst = false;
	}
	else
	{
		digitalWrite(1, LOW);
		displayValues(display2State);
		digitalWrite(0, HIGH);
		displayFirst = true;
	}
}

void assignNumberToDisplays()
{
	assignSingleDigit((numberToDisplay - numberToDisplay % 10) / 10, display1State);
	assignSingleDigit(numberToDisplay % 10, display2State);
}

void assignSingleDigit(int number, bool displayState[])
{
    switch (number)
    {
		case 0:
			assignDisplayState(1, 1, 1, 1, 1, 1, 0, displayState);
			break;
		case 1:
			assignDisplayState(0, 1, 1, 0, 0, 0, 0, displayState);
			break;
		case 2:
			assignDisplayState(1, 1, 0, 1, 1, 0, 1, displayState);
			break;
		case 3:
			assignDisplayState(1, 1, 1, 1, 0, 0, 1, displayState);
			break;
		case 4:
			assignDisplayState(0, 1, 1, 0, 0, 1, 1, displayState);
			break;
		case 5:
			assignDisplayState(1, 0, 1, 1, 0, 1, 1, displayState);
			break;
		case 6:
			assignDisplayState(1, 0, 1, 1, 1, 1, 1, displayState);
			break;
		case 7:
			assignDisplayState(1, 1, 1, 0, 0, 0, 0, displayState);
			break;
		case 8:
			assignDisplayState(1, 1, 1, 1, 1, 1, 1, displayState);
			break;
		case 9:
			assignDisplayState(1, 1, 1, 1, 0, 1, 1, displayState);
			break;
		default: break;
    }
}

void assignDisplayState(bool a, bool b, bool c, bool d, bool e, bool f, bool g, bool displayState[])
{
 	displayState[0] = a;
 	displayState[1] = b;
 	displayState[2] = c;
 	displayState[3] = d;
 	displayState[4] = e;
 	displayState[5] = f;
 	displayState[6] = g;
}

void displayValues(bool displayState[])
{
	digitalWrite(7, displayState[0]);
  	digitalWrite(6, displayState[1]);
	digitalWrite(3, displayState[2]);
	digitalWrite(4, displayState[3]);
	digitalWrite(5, displayState[4]);
	digitalWrite(8, displayState[5]);
	digitalWrite(9, displayState[6]);
}