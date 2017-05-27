/*
  TetrisBoard.cpp - Tetris Board library
  Copyright (c) 2017 Spencer Greene
*/


#include <Arduino.h>
#include "TetrisBoard.h"

uint8_t const tet[7][4][4][4] PROGMEM = {
{{0, 0, 1, 0,  // I in 4 rotations
  0, 0, 1, 0,
  0, 0, 1, 0,
  0, 0, 1, 0},

 {0, 0, 0, 0,
  0, 0, 0, 0,
  1, 1, 1, 1,
  0, 0, 0, 0},
  
 {0, 0, 1, 0,
  0, 0, 1, 0,
  0, 0, 1, 0,
  0, 0, 1, 0},

 {0, 0, 0, 0,
  0, 0, 0, 0,
  1, 1, 1, 1,
  0, 0, 0, 0}
},

{{0, 0, 0, 0,  // O in 4 rotations
  0, 0, 0, 0,
  0, 1, 1, 0,
  0, 1, 1, 0},

 {0, 0, 0, 0,
  0, 0, 0, 0,
  0, 1, 1, 0,
  0, 1, 1, 0},
  
 {0, 0, 0, 0,
  0, 0, 0, 0,
  0, 1, 1, 0,
  0, 1, 1, 0},
  
 {0, 0, 0, 0,
  0, 1, 1, 0,
  0, 1, 1, 0,
  0, 0, 0, 0}
},

{{0, 0, 0, 0,  // T in 4 rotations
  0, 0, 0, 0,
  0, 1, 1, 1,
  0, 0, 1, 0},

 {0, 0, 0, 0,
  0, 0, 0, 1,
  0, 0, 1, 1,
  0, 0, 0, 1},
  
 {0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 1, 0,
  0, 1, 1, 1},
  
 {0, 0, 0, 0,
  0, 0, 1, 0,
  0, 0, 1, 1,
  0, 0, 1, 0}
},

{{0, 0, 0, 0,  // J in 4 rotations
  0, 0, 1, 0,
  0, 0, 1, 0,
  0, 1, 1, 0},

 {0, 0, 0, 0,
  0, 0, 0, 0,
  1, 0, 0, 0,
  1, 1, 1, 0},
  
 {0, 0, 0, 0,
  0, 1, 1, 0,
  0, 1, 0, 0,
  0, 1, 0, 0},
  
 {0, 0, 0, 0,
  0, 0, 0, 0,
  1, 1, 1, 0,
  0, 0, 1, 0}
},

{{0, 0, 0, 0,  // L in 4 rotations
  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 1, 1, 0},

 {0, 0, 0, 0,
  0, 0, 0, 0,
  0, 1, 1, 1,
  0, 1, 0, 0},
  
 {0, 0, 0, 0,
  0, 1, 1, 0,
  0, 0, 1, 0,
  0, 0, 1, 0},
  
 {0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 1,
  0, 1, 1, 1}
},

{{0, 0, 0, 0,  // S in 4 rotations
  0, 0, 0, 0,
  0, 0, 1, 1,
  0, 1, 1, 0},

 {0, 0, 0, 0,
  0, 0, 1, 0,
  0, 0, 1, 1,
  0, 0, 0, 1},
  
 {0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 1, 1,
  0, 1, 1, 0},
  
 {0, 0, 0, 0,
  0, 0, 1, 0,
  0, 0, 1, 1,
  0, 0, 0, 1}
},

{{0, 0, 0, 0,  // Z in 4 rotations
  0, 0, 0, 0,
  0, 1, 1, 0,
  0, 0, 1, 1},

 {0, 0, 0, 0,
  0, 0, 0, 1,
  0, 0, 1, 1,
  0, 0, 1, 0},
  
 {0, 0, 0, 0,
  0, 0, 0, 0,
  0, 1, 1, 0,
  0, 0, 1, 1},
  
 {0, 0, 0, 0,
  0, 0, 0, 1,
  0, 0, 1, 1,
  0, 0, 1, 0}
}
  
};

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

int TetrisBoard::tetMinX() {
  int x = 0;
  while (x < 4) {
    for (int y = 0; y < 4; y++) {
      if (tet[_tetType][_tetRot][x][y]) {
        return (x + _tetX);
      }
    }
    x++;
  }
  Serial.println("tetMinX() ERROR! tet 4x4 array was empty");
  return (_tetX);
}

int TetrisBoard::tetMinY() {
  int y = 0;
  while (y < 4) {
    for (int x = 0; x < 4; x++) {
      if (tet[_tetType][_tetRot][x][y]) {
        return (y + _tetY);
      }
    }
    y++;
  }
  Serial.println("tetMinY() ERROR! tet 4x4 array was empty");
  return (_tetY);
}

int TetrisBoard::tetMaxX() {
  int x = 3;
  while (x >= 0) {
    for (int y = 0; y < 4; y++) {
      if (tet[_tetType][_tetRot][x][y]) {
        return (x + _tetX);
      }
    }
    x--;
  }
  Serial.println("tetMaxX() ERROR! tet 4x4 array was empty");
  return (_tetX + 3);
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
