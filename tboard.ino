// Created 26 May 2017

#include "TetrisBoard.h"
// #include <OctoWS2811.h>

TetrisBoard game;

void setup()
{
  pinMode(13, OUTPUT);
  game.setupLEDs();
}

void loop()
{
  // piece type 5 in default rotation, at loc 10x20, color=3
  game.placeTet(5, 2, 20, 3); 	
  Serial.println("placed at 2, 20");
  game.dump();
  game.freezeTet();
  while(1);
  game.dump();
  game.placeTet(4, 10, 20, 2);
  game.dump();
  game.drawTet();
  delay(1000);
  
  game.rotTet();
  game.dump();
  game.drawTet();
  delay(1000);
  
  bool success = game.moveTet(0, -1);
  if (success) {
    Serial.print("ok: ");
  } else {
    Serial.print("xx: ");
  }
  Serial.println("moved down 1");
  game.dump();
  game.drawTet();
  delay(1000);
  
  while(1);
}



