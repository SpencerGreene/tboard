// Created 26 May 2017

#include "TetrisBoard.h"

// teensy pinout
#define BUT_ROT 22
#define BUT_LF  1
#define BUT_RT  17
#define BUT_DRP 18
#define UNCONNECTED 0

#define PRESSED 0       // for pullup buttons
#define NOTPRESSED 1

#define STEPTIME 200    // in msec
#define BUTTONTIME  50     // time between button presses

TetrisBoard game;
uint32_t moveTime;
uint32_t inputTime;
bool keepGoing;

bool button_lf = NOTPRESSED;
bool button_rt = NOTPRESSED;
bool button_rot = NOTPRESSED;
bool button_drp = NOTPRESSED;

void setup()
{
  pinMode(UNCONNECTED, INPUT);
  pinMode(BUT_LF, INPUT_PULLUP);
  pinMode(BUT_RT, INPUT_PULLUP);
  pinMode(BUT_ROT, INPUT_PULLUP);
  pinMode(BUT_DRP, INPUT_PULLUP);

  randomSeed(analogRead(UNCONNECTED));

  game.setupLEDs();
  game.spawn();
  moveTime = millis();
  inputTime = millis();
}

// return the number of whichever button went from unpressed to pressed
// if more than one button, select randomly among them
int readButtons() {
  int b;
  int changed = 0;

  b = digitalRead(BUT_LF);
  if (b != button_lf) {
    if (b == NOTPRESSED) 
      button_lf = b;
    else
      changed |= 1; 
  }

  b = digitalRead(BUT_RT);
  if (b != button_rt) {
    if (b == NOTPRESSED)
      button_rt = b;
    else
      changed |= 2;
  }

  b = digitalRead(BUT_ROT);
  if (b != button_rot) {
    if (b == NOTPRESSED)
      button_rot = b;
    else
      changed |= 4;
  }

  b = digitalRead(BUT_DRP);
  if (b != button_drp) {
    if (b == NOTPRESSED)
      button_drp = b;
    else
      changed |= 8;
  }

  changed = button_resolve(changed);
  
  switch (changed) {
      case 0:
        return(-1);
      case 1:
        button_lf = PRESSED;
        return BUT_LF;
      case 2:
        button_rt = PRESSED;
        return BUT_RT;
      case 4:
        button_rot = PRESSED;
        return BUT_ROT;
      case 8:
        button_drp = PRESSED;
        return BUT_DRP;
    }

    Serial.println("readButtons error!");
    return(-1);
}

// resolve button conflicts by returning a bitmask with only one hot
// 1=left, 2=right, 4=rot, 8=drop
int button_resolve(int changed)
{
    if (changed == 0 || changed == 1 || changed == 2 || changed ==4 || changed == 8) return changed;

    // rotate is always first
    if (changed & 4) return 4;
    
    // drop is always last
    if (changed & B1000) changed &= B0111;

    // resolve lf/rt randomly 
    if ((changed & B0001) && (changed & B0010)) {
      int resolve = random(2);
      if (resolve) return 2; else return 1;
    }

    return changed;
}

void loop()
{
  if (millis() > inputTime + BUTTONTIME) {
    inputTime = millis();
    switch(readButtons()) {
      case -1:
        break;
      case BUT_LF:
        Serial.println("move left");
        break;
      case BUT_RT:
        Serial.println("move right");
        break;
      case BUT_ROT:
        Serial.println("rotate");
        game.rotTet();
        break;
      case BUT_DRP:
        Serial.println("drop");
        break;
      default:
        break;
    }
  }
  if (millis() > moveTime + STEPTIME) {
    moveTime += STEPTIME;
    keepGoing = game.moveTet(0, -1);
    if (keepGoing == 0) {
      game.freezeTet();
      keepGoing = game.spawn(); 
      if (keepGoing == 0) {
        game.gameOver();
        while(1);
      }
    }
  }
}



