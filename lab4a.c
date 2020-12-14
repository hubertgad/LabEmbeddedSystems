#include <LiquidCrystal.h>

LiquidCrystal lcd(13, 12, 11, 10, 1, 0);

byte chicken[8] ={
                      B01110,
                      B01011,
                      B01110,
                      B00100,
                      B01110,
                      B10101,
                      B00100,
                      B01010
                     };
byte home[8] ={
                      B00100,
                      B01110,
                      B11111,
                      B01110,
                      B01110,
                      B01110,
                      B01010,
                      B01010
                     };

int chickenX, chickenY, newChickenX, newChickenY;

bool isGameOver = false;

void setup()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  
  lcd.createChar(0, chicken);
  lcd.createChar(1, home);
  startGame();
}

void startGame()
{
  chickenX = 0;
  chickenY = 1;
  newChickenX = 0;
  newChickenY = 1;
  
  lcd.begin(16, 2);
  lcd.print("let chicken");
  lcd.setCursor(0, 1);
  lcd.print("go home game");
  delay(1500);
  lcd.setCursor(15, 0);
  lcd.write(byte(1));
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("           ");
  lcd.setCursor(0, 1);
  lcd.print("            ");
  delay(500);
  printChicken();   
}

void loop()
{
  readChickenPos();
  
  if (newChickenX != chickenX || newChickenY != chickenY)
  {
  	eraseChicken();
    chickenX = newChickenX;
    chickenY = newChickenY;
  	printChicken();
    
    if (chickenX == 15 && chickenY == 0)
    {
      overTheGame();
    }
  }
  
  delay(200);
}

void printChicken()
{
  lcd.setCursor(chickenX, chickenY);
  lcd.write(byte(0));
}

void eraseChicken()
{
  lcd.setCursor(chickenX, chickenY);
  lcd.print(" ");
}

void overTheGame()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("you have won!");
  lcd.setCursor(0, 1);
  lcd.print("awesome!!");
  delay(10000);
  startGame();
}

void readChickenPos()
{
  switch (analogRead(A0) / 128)
  {
    case 1:
    {
      if (chickenX > 0) newChickenX--;
      break;
    }
    case 2:
    {
      if (chickenY > 0) newChickenY--;
      break;
    }
    case 3:
    {
      if (chickenX < 15) newChickenX++;
      break;
    }
    case 4:
    {
      if (chickenY < 1) newChickenY++;
      break;
    }
    default: break;
  }
}