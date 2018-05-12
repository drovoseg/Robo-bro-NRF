#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "SendEngineState.h"


#define FORWARD 2 // A button
#define RIGHT 3 // B button
#define BACK 4 // C button
#define LEFT 5 // D button

const byte COMMAND_LEFT =  1;
const byte COMMAND_FORWARD =  2;
const byte COMMAND_RIGHT =  3;
const byte COMMAND_BACKWARD =  4;

byte oldCommand = 0;



void setup()
{
  Serial.begin(9600);
  printf_begin();//привязка printf() к Serial


  //илициализация управляющих кнопок
  pinMode(FORWARD, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(BACK, INPUT);
  pinMode(LEFT, INPUT);

  initNRF();
}

//engine info
// 0 left direction
// 1 right direction
// 2 left power
// 3 right power


//void sendEngineState(byte leftState, byte leftPower, byte rightState, byte rightPower) {
void sendForward() {
  sendEngineState(1, 255, 1, 255);
}

void sendBack() {
  sendEngineState(0, 255, 0, 255);
}

void sendLeft() {
  sendEngineState(0, 255, 1, 255);
}

void sendRight() {
  sendEngineState(1, 255, 0, 255);
}

void sendStop() {
  sendEngineState(0, 0, 0, 0);
}

void sendXY(long x, long y) {
  byte leftState;
  byte leftPower;
  byte rightState;
  byte rightPower;


  if (y >= 512) {
    leftState = rightState = 1;
  } else {
    leftState = rightState = 0;
  }

  long defaultX = 0;
  long defaultY = 512;

  long deltaX = abs(x - 512);
  long deltaY = abs(y - 512);

  double angle = abs(deltaX * defaultX + deltaY * defaultY)
                 / (sqrt(defaultX * defaultX + defaultY * defaultY) * sqrt(deltaX * deltaX + deltaY * deltaY));

  double rads = acos(angle);

  long tX = x; long tY = y;
  tX += 1023 - max(x, y);
  tY += 1023 - max(x, y);

  double power = sqrt((x - 512) * (x - 512) + (y - 512) * (y - 512));
  double power2 = sqrt((tX - 512) * (tX - 512) + (tY - 512) * (tY - 512));

  double k = (power / power2);
  if(k > 1)
    k = 1;

  k = pow(k, 10);
  
  if (x > 512) {
    rightPower = 255 * (1 - (rads / 1.57)) * k;
    leftPower = 255 * k;
  } else {
    leftPower = 255 *  (1 - (rads / 1.57)) * k;
    rightPower = 255 * k;
  }

//  Serial.println(x);
//  Serial.println(k);
//  Serial.println(leftState);
//  Serial.println(leftPower);
//
//  Serial.println(rightState);
//  Serial.println(rightPower);
//
//  
//  Serial.println();

  sendEngineState(leftState, leftPower, rightState, rightPower);
}


bool anyButtonPressed = false;
bool wasPressedOnLastTick = false;
long lastStopTime = -999999;
void loop()
{
  anyButtonPressed = false;
  if (!digitalRead(FORWARD))
  {
    if (oldCommand != COMMAND_FORWARD) {
      printf("FORWARD\n");
      sendForward();
    }
    oldCommand = COMMAND_FORWARD;
    anyButtonPressed = true;
  }
  else if (!digitalRead(RIGHT))
  {
    if (oldCommand != COMMAND_RIGHT) {
      printf("RIGHT\n");
      sendRight();
    }
    oldCommand = COMMAND_RIGHT;
    anyButtonPressed = true;
  }
  else if (!digitalRead(BACK))
  {
    if (oldCommand != COMMAND_BACKWARD) {
      printf("BACK\n");
      sendBack();
    }
    oldCommand = COMMAND_BACKWARD;
    anyButtonPressed = true;
  }
  else if (!digitalRead(LEFT))
  {
    if (oldCommand != COMMAND_LEFT) {
      printf("LEFT\n");
      sendLeft();
    }
    oldCommand = COMMAND_LEFT;
    anyButtonPressed = true;
  } else {
    int x = analogRead(0);
    int y = analogRead(1);

   
    if (x > 515 || x < 508 || y > 515 || y < 508) {
      sendXY(x, y);
      anyButtonPressed = true;
    }   
  }
  if (!anyButtonPressed
      && !wasPressedOnLastTick
      && millis() - lastStopTime > 3000) {
    sendStop();
    printf("STOP timer\n");
    lastStopTime = millis();
  }

  if (!anyButtonPressed && wasPressedOnLastTick) {
    sendStop();
    printf("STOP released\n");
    lastStopTime = millis();
  }

  if (!anyButtonPressed) {
    oldCommand = 0;
  }

  wasPressedOnLastTick = anyButtonPressed;
}
