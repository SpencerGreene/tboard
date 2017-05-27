/*
  TetrisBoard.cpp - Tetris Board library
  Copyright (c) 2017 Spencer Greene
*/


#include <Arduino.h>
#include "TetrisBoard.h"


// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

TetrisBoard::TetrisBoard()
{
  // initialize this instance's variables

  for (int x = 0; x < DIMX; x++) {
    for (int y = 0; y < DIMY; y++) {
      board[x][y] = 0;
    }
  }
}

void TetrisBoard::placeTet(int tetType, int tetX, int tetY, int tetColor)
{
  _tetType = tetType;
  _tetX = tetX;
  _tetY = tetY;
  _tetRot = 0;
  _tetColor = tetColor;
  Serial.print(tetType);
  Serial.print(" at ");
  Serial.print(tetX);
  Serial.print(",");
  Serial.print(tetY);
  Serial.println(" placeTet");
}

void TetrisBoard::rotTet(void)
{
  Serial.println("rotTet");
}

void TetrisBoard::dump(void)
{
  Serial.println("dump");
}

bool TetrisBoard::moveTet(int xOffset, int yOffset)
{
  Serial.print(xOffset);
  Serial.print(",");
  Serial.print(yOffset);
  Serial.println(" moveDown");
  return(1);
}

void TetrisBoard::freezeTet(void)
{
  Serial.println("freezeTet");
}
