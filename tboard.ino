#include "TetrisBoard.h"

// Demostrates how to do something with the TetrisBoard library

// Created 26 May 2017

TetrisBoard board = TetrisBoard();

void setup()
{

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
  board.rotTet();
  board.dump();
  bool success = board.moveTet(0, -1);
  if (success) {
    Serial.print("ok: ");
  } else {
    Serial.print("xx: ");
  }
  Serial.println("moved down 1");
  board.dump();
  while(1);
}

