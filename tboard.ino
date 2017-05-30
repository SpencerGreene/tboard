// Created 26 May 2017

#include "TetrisBoard.h"
// #include <OctoWS2811.h>

TetrisBoard board = TetrisBoard();

void setup()
{
  pinMode(13, OUTPUT);
  board.init();
}

void loop()
{
  // piece type 5 in default rotation, at loc 10x20, color=3
  board.placeTet(5, 2, 20, 3); 	
  Serial.println("placed at 2, 20");
  board.dump();
  board.freezeTet();
  while(1);
  board.dump();
  board.placeTet(4, 10, 20, 2);
  board.dump();
  board.drawTet();
  delay(1000);
  
  board.rotTet();
  board.dump();
  board.drawTet();
  delay(1000);
  
  bool success = board.moveTet(0, -1);
  if (success) {
    Serial.print("ok: ");
  } else {
    Serial.print("xx: ");
  }
  Serial.println("moved down 1");
  board.dump();
  board.drawTet();
  delay(1000);
  
  while(1);
}



