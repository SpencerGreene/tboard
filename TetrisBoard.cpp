/*
  TetrisBoard.cpp - Tetris Board library
  Copyright (c) 2017 Spencer Greene
*/


#include <Arduino.h>
#include <OctoWS2811.h>
#include "TetrisBoard.h"

DMAMEM int displayMemory[LEDS_PER_STRIP*6];
int drawingMemory[LEDS_PER_STRIP*6];
OctoWS2811 leds(LEDS_PER_STRIP, displayMemory, drawingMemory, config);

uint32_t const colors[] = { 0xff0000, 0x00ff00, 0x0000ff, 0x888800, 0x880088, 0x008888, 0xCC4400, 0x44cc00, 0xCC0044, 0x4400cc, 0x00cc44, 0x0044cc };

uint8_t const tetrimino[7][4][4][4] PROGMEM = {
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

void TetrisBoard::init()
{
  leds.begin();
  leds.show();
}

void TetrisBoard::adjustTet()
{
  int minX = tetMinX();
  if (minX < 0) {
    _tetX -= minX;
    Serial.print(minX);
    Serial.println(" placeTet adjusting right");
  }
  int maxX = tetMaxX();
  if (maxX >= DIMX) {
    _tetX += (DIMX - maxX - 1);
    Serial.print(DIMX - maxX - 1);
    Serial.println(" placeTet adjusting left");
  }
}

void TetrisBoard::eraseTet()
{
  paintTet(0);
}

void TetrisBoard::drawTet()
{
  paintTet(colors[_tetColor]);
}

void TetrisBoard::paintTet(uint32_t c)
{
  for (int x = _tetX; x < _tetX + 4; x++) {
    for (int y = _tetY; y < _tetY + 4; y++) {
      if (isInTet(x, y)) {
        drawBlock(x, y, c);
      }
    }
  }
}

void TetrisBoard::drawBlock(int x, int y, uint32_t c)
{
  
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
  adjustTet();
}

int TetrisBoard::tetMinX() {
  int x = 0;
  while (x < 4) {
    for (int y = 0; y < 4; y++) {
      if (tetrimino[_tetType][_tetRot][x][y]) {
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
      if (tetrimino[_tetType][_tetRot][x][y]) {
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
      if (tetrimino[_tetType][_tetRot][x][y]) {
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

bool TetrisBoard::isInTet(int x, int y)
{
  int xOffset = x - _tetX;
  int yOffset = y - _tetY;

  if (xOffset < 0 || yOffset < 0 || xOffset >= DIMX || yOffset >= DIMY) {
    return 0;
  }
  return tetrimino[_tetType][_tetRot][xOffset][yOffset];
}

int TetrisBoard::dimmer(int color, int percent) {
  int r = (color & 0xFF0000) >> 16;
  int g = (color & 0x00FF00) >> 8;
  int b = color & 0xFF;

  r = (r*percent)/100;
  g = (g*percent)/100;
  b = (b*percent)/100;

  int output = (r<<16) + (g<<8) + b;
  return(output);
}

void TetrisBoard::setxy(int x, int y, uint32_t c) {
  leds.setPixel(led_map(x, y), dimmer(c, LIGHT_LEVEL));
}


int TetrisBoard::led_map(int x,int y) {
  int output; 

  x = (COLS_LEDs-1) - x;
  y = (ROWS_LEDs-1) - y;
  
  if (y%2 == 1) {
    output = ((y+1)*COLS_LEDs-x-1);
  } else {
    output = (y*COLS_LEDs+x);
  }
  
  return output;
}
