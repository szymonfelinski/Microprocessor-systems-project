#include <avr/wdt.h>

const int plusPin[] = {2, 4, 13}; //always same size az minusPin
const int minusPin[] = {6, 5, 3}; //always same size as plusPin
const int analogInPin = A0;

const int brightness = 30; //brightness od LEDs (0-255)

int LEDMatrix[sizeof(plusPin)][sizeof(minusPin)];
int SimonTable[sizeof(plusPin)][sizeof(minusPin)];

bool gameBegun = false; //check if game has begun
const int SimonSize = 4; //number of steps in game
int stepNumber = SimonSize;

void setup() {
  randomSeed(analogRead(3));
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  for (int i = sizeof(plusPin); i >= 0; i--)
  {
    pinMode(plusPin[i], OUTPUT); //initialize anode pins
  }
  for (int i = sizeof(minusPin); i >= 0; i--)
  {
    pinMode(minusPin[i], OUTPUT); //initialize cathode pins
  }
  memset(LEDMatrix, 0, sizeof(LEDMatrix)); //initialize as 0's
  memset(SimonTable, 0, sizeof(SimonTable)); //initialize as 0's
  while (!createSimon()) {}
  while (!Serial) {}
}

void softwareReset() {
  // start watchdog with the provided prescaler
  wdt_enable(WDTO_60MS);
  // wait for the prescaler time to expire
  // without sending the reset signal by using
  // the wdt_reset() method
  while (1) {}
}

void clearLED()
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(plusPin[i], LOW);
    digitalWrite(minusPin[i], HIGH);
  }
  memset(LEDMatrix, 0, sizeof(LEDMatrix));
}

void sweepLED()
{
  for (int i = 0; i < sizeof(plusPin); i++)
  {
    for (int j = 0; j < sizeof(minusPin); j++)
    {
      LEDMatrix[i][j] = 1;
      updateLED(200);
    }
  }
}

void updateLED(int dispTime)
{
  for (int i = 0; i < sizeof(plusPin); i++)
  {
    for (int j = 0; j < sizeof(minusPin); j++)
    {
      if (LEDMatrix[i][j])
      {
        digitalWrite(plusPin[i], HIGH);
        analogWrite(minusPin[j], 255 - brightness);
        //break;
      }
    }
  }
  delay(dispTime); //freeze display for a set amount of miliseconds
  clearLED(); //reset the LED matrix to 0's
}

void checkLED(int buttonValue)
{
  if (buttonValue >= 920 && buttonValue <= 940)
  {
    LEDMatrix[0][0] = 1;
  }
  else if (buttonValue >= 830 && buttonValue <= 850)
  {
    LEDMatrix[0][1] = 1;
  }
  else if (buttonValue >= 685 && buttonValue <= 705)
  {
    LEDMatrix[0][2] = 1;
  }
  else if (buttonValue >= 450 && buttonValue <= 480)
  {
    LEDMatrix[1][0] = 1;
  }
  else if (buttonValue >= 398 && buttonValue <= 418)
  {
    LEDMatrix[1][1] = 1;
  }
  else if (buttonValue >= 311 && buttonValue <= 331)
  {
    LEDMatrix[1][2] = 1;
  }
  else if (buttonValue >= 226 && buttonValue <= 246)
  {
    LEDMatrix[2][0] = 1;
  }
  else if (buttonValue >= 167 && buttonValue <= 187)
  {
    LEDMatrix[2][1] = 1;
  }
  else if (buttonValue >= 82 && buttonValue <= 102)
  {
    LEDMatrix[2][2] = 1;
  }
  if (buttonValue > 82)
  {
    Serial.println(buttonValue);
    checkButtonToSimon();
  }
  updateLED(100);

}

void showLetter(int showTime)
{
  int showInterval = 2; //interval between loops
  for (int i = 0; i < showTime; i++)
  {
    for (int i = 0; i < sizeof(plusPin); i++)
    {
      for (int j = 0; j < sizeof(minusPin); j++)
      {
        if (LEDMatrix[i][j])
        {
          digitalWrite(plusPin[i], HIGH);
          analogWrite(minusPin[j], 255 - brightness);
          delay(showInterval);
          digitalWrite(plusPin[i], LOW);
          digitalWrite(minusPin[j], HIGH);
          //break;
        }
      }
    }
  }
  clearLED();
}

void showO(int showTime)
{
  clearLED();
  LEDMatrix[0][1] = 1;
  LEDMatrix[1][0] = 1;
  LEDMatrix[1][2] = 1;
  LEDMatrix[2][1] = 1;
  showLetter(showTime);
}

void showK(int showTime)
{
  clearLED();
  LEDMatrix[0][0] = 1;
  LEDMatrix[0][2] = 1;
  LEDMatrix[1][1] = 1;
  LEDMatrix[2][0] = 1;
  LEDMatrix[2][1] = 1;
  LEDMatrix[2][2] = 1;
  showLetter(showTime);
}

void showX(int showTime)
{
  clearLED();
  LEDMatrix[0][0] = 1;
  LEDMatrix[0][2] = 1;
  LEDMatrix[1][1] = 1;
  LEDMatrix[2][0] = 1;
  LEDMatrix[2][2] = 1;
  showLetter(showTime);
}

void printLED()
{
  Serial.println("Tablica LED:");
  Serial.print(LEDMatrix[0][0]);
  Serial.print(LEDMatrix[0][1]);
  Serial.println(LEDMatrix[0][2]);
  Serial.print(LEDMatrix[1][0]);
  Serial.print(LEDMatrix[1][1]);
  Serial.println(LEDMatrix[1][2]);
  Serial.print(LEDMatrix[2][0]);
  Serial.print(LEDMatrix[2][1]);
  Serial.println(LEDMatrix[2][2]);
}
void printSimon()
{
  Serial.println("Tablica Sajmona:");
  Serial.print(SimonTable[0][0]);
  Serial.print(SimonTable[0][1]);
  Serial.println(SimonTable[0][2]);
  Serial.print(SimonTable[1][0]);
  Serial.print(SimonTable[1][1]);
  Serial.println(SimonTable[1][2]);
  Serial.print(SimonTable[2][0]);
  Serial.print(SimonTable[2][1]);
  Serial.println(SimonTable[2][2]);
}

bool createSimon()
{
  int toMake = SimonSize;
  while (toMake)
  {
    //Serial.print(toMake);
    //Serial.println("aaa");
    int row = random() % 3;
    int column = random() % 3;
    if (!SimonTable[row][column])
    {
      SimonTable[row][column] = toMake;
      toMake--;
      //Serial.print(toMake);
      //Serial.println("zrobilem linie");
    }

  }
  printSimon();
  return true;
}

bool waitForStart(int buttonValue)
{
  LEDMatrix[1][1] = 1;
  int blinkTime = 100;
  updateLED(blinkTime);
  updateLED(blinkTime);
  if (buttonValue >= 398 && buttonValue <= 418)
    return true;
  else
    return false;
}

bool beginGame(int counter)
{
  bool cnt = 1;
  for (int i = 0; i < sizeof(plusPin); i++)
  {
    if (cnt)
      for (int j = 0; j < sizeof(minusPin); j++)
      {
        if (SimonTable[i][j] == counter && counter >= 1)
        {
          LEDMatrix[i][j] = 1;
          //printLED();
          updateLED(1000);
          //Serial.print(counter);
          beginGame(--counter);
          cnt = 0;
          //Serial.println("wyświetlenie");
        }
      }
    else break;
  }
  //printSimon();
  return true;
}

void winGame() //code when game win here
{
  int showTime=60;
  //sweepLED();
  for (int i=1;i<5;i++) // show OK set amount of times
  {
    showO(showTime);
    showK(showTime);
  }
  softwareReset();
  
}

void loseGame()
{
  printLED();
  showX(500);
  softwareReset();
}

void checkButtonToSimon()
{
  for (int i = 0; i < sizeof(plusPin); i++)
  {
    for (int j = 0; j < sizeof(minusPin); j++)
    {
      if (SimonTable[i][j] == stepNumber)
      {
        if (LEDMatrix[i][j])
        {
          Serial.println("is gut");
          stepNumber--;
          delay(200);
          return;
        }
        else
        {
          loseGame();
        }
      }

    }
  }
}

void loop() {
  int buttonValue = analogRead(analogInPin);        // value read from buttons
  Serial.println(buttonValue);
  if (!gameBegun) //one-time run
  {
    clearLED(); //reset the LED matrix to 0's
    if (waitForStart(buttonValue))
      gameBegun = beginGame(SimonSize); //start game if it's not been started already
  }
  else checkLED(buttonValue); //check if any one button is pressed
  if (!stepNumber) winGame();
}
