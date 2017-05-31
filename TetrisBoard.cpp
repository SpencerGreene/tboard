/*
  TetrisBoard.cpp - Tetris Board library
  Copyright (c) 2017 Spencer Greene
*/

#define VERBOSE 0

#include <Arduino.h>
#include <OctoWS2811.h>
#include "TetrisBoard.h"


#define BLOCK_XOFFSET 10
#define BLOCK_YOFFSET 0
#define BLOCK_SIZE 2
#define ROTATED 0

/*
#define BLOCK_XOFFSET 3
#define BLOCK_YOFFSET 0
#define BLOCK_SIZE 2
#define ROTATED 1
// XX need to change DIMX as well, 36 only permits 18 wide
*/

DMAMEM int displayMemory[LEDS_PER_STRIP*6];
int drawingMemory[LEDS_PER_STRIP*6];
OctoWS2811 leds(LEDS_PER_STRIP, displayMemory, drawingMemory, config);

uint32_t const colors[] = { 0, 0xff0000, 0x00ff00, 0x0000ff, 0x888800, 0x880088, 0x008888, 0xCC4400, 0x44cc00, 0xCC0044, 0x4400cc, 0x00cc44, 0x0044cc };

uint8_t const tetrimino[7][4][4][4] PROGMEM = {
{{0, 0, 1, 0,  // I in 4 rotations
  0, 0, 1, 0,
  0, 0, 1, 0,
  0, 0, 1, 0},

 {0, 0, 0, 0,
  1, 1, 1, 1,
  0, 0, 0, 0,
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

{{0, 0, 0, 0,   // J in 4 rotations
  0, 1, 1, 0,
  0, 1, 0, 0,
  0, 1, 0, 0},
  
 {0, 0, 0, 0,
  0, 0, 0, 0,
  1, 1, 1, 0,
  0, 0, 1, 0},
  
 {0, 0, 0, 0,  
  0, 0, 1, 0,
  0, 0, 1, 0,
  0, 1, 1, 0},

 {0, 0, 0, 0,
  0, 0, 0, 0,
  1, 0, 0, 0,
  1, 1, 1, 0}
},

{{0, 0, 0, 0,   // L in 4 rotations
  0, 1, 1, 0,
  0, 0, 1, 0,
  0, 0, 1, 0},
  
 {0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 1,
  0, 1, 1, 1},
  
 {0, 0, 0, 0,  
  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 1, 1, 0},

 {0, 0, 0, 0,
  0, 0, 0, 0,
  0, 1, 1, 1,
  0, 1, 0, 0}
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

void TetrisBoard::setupLEDs()
{
  delay(100);
  leds.begin();
  leds.show();
  for (int y = 0; y < DIMY; y++) {
    paintBlock(-1, y, 0xffffff);
    paintBlock(DIMX, y, 0xffffff);
  }
  leds.show();
}

void TetrisBoard::adjustTet()
{
  int minX = tetMinX();
  if (minX < 0) {
    _tetX -= minX;
    Serial.print(minX);
    Serial.println(" adjustTet adjusting right");
  }
  int maxX = tetMaxX();
  if (maxX >= DIMX) {
    _tetX += (DIMX - maxX - 1);
    Serial.print(DIMX - maxX - 1);
    Serial.println(" adjustTet adjusting left");
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
  adjustTet();
  drawTet();
}

int TetrisBoard::tetMinX() {
  int x = 0;
  while (x < 4) {
    for (int y = 0; y < 4; y++) {
      if (tetrimino[_tetType][_tetRot][y][x]) {
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
      if (tetrimino[_tetType][_tetRot][y][x]) {
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
      if (tetrimino[_tetType][_tetRot][y][x]) {
        return (x + _tetX);
      }
    }
    x--;
  }
  Serial.println("tetMaxX() ERROR! tet 4x4 array was empty");
  return (_tetX + 3);
}


bool TetrisBoard::isInTet(int x, int y)
{
  int xOffset = x - _tetX;
  int yOffset = y - _tetY;

  if (xOffset < 0 || yOffset < 0 || xOffset >= 4 || yOffset >= 4) {
    return 0;
  }
  return tetrimino[_tetType][_tetRot][yOffset][xOffset];
}

bool TetrisBoard::isOnBoard(int x, int y)
{
  return (x >= 0 && y >= 0 && x < DIMX && y < DIMY);
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
  if (x < 0 || x >= COLS_LEDs || y < 0 || y > ROWS_LEDs) {
    return;
  }
  
  leds.setPixel(led_map(x, y), dimmer(c, LIGHT_LEVEL));
  if (VERBOSE) {
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print("=");
    Serial.print(led_map(x,y));
    Serial.print("<-");
    Serial.print(c);
    Serial.print(":");
    Serial.print(dimmer(c, LIGHT_LEVEL));
    Serial.println(" setxy");
  }
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
        paintBlock(x, y, c);
      }
    }
  }
  leds.show();
}

void TetrisBoard::paintBlock(int x, int y, uint32_t c)
{
  if (VERBOSE) {
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print("<-");
    Serial.print(c);
    Serial.println(" paintBlock");
  }

  // we need to paint left/right borders at x=-1, x=DIMX, so don't restrict to isOnBoard
  // if (isOnBoard(x, y) == 0) return;
  
  int xo = BLOCK_XOFFSET + BLOCK_SIZE * x;
  int yo = BLOCK_YOFFSET + BLOCK_SIZE * y;
        
  for (int xi = 0; xi < BLOCK_SIZE; xi++) {
    for (int yi = 0; yi < BLOCK_SIZE; yi++) {
      if (ROTATED) {
        setxy(yi + yo, ROWS_LEDs - 1 - (xi + xo), c);
      } else {
        setxy(xi + xo, yi + yo, c);
      }
    }
  }
}


bool TetrisBoard::moveTet(int xOffset, int yOffset)
{
  if (VERBOSE) {
    Serial.print(xOffset);
    Serial.print(",");
    Serial.print(yOffset);
    Serial.println(" moveTet");
  }
  
  eraseTet();
  _tetX += xOffset;
  _tetY += yOffset;
  if (tetCollide()) {
    _tetX -= xOffset;
    _tetY -= yOffset;
    drawTet();
    return 0;
  }
  
  drawTet();
  return(1);
}

void TetrisBoard::freezeTet(void)
{
  for (int x = _tetX; x < _tetX + 4; x++) {
    for (int y = _tetY; y < _tetY + 4; y++) {
      if (isInTet(x, y)) {
        board[x][y] = _tetColor;
      }
    }
  }
  int row;
  while ((row = lowestFullRow()) > -1) {
    killRow(row);
  }

  if (VERBOSE) {
    Serial.println("freezeTet");
    for (int i = DIMY - 1; i >= 0; i--) {
      for (int j = 0; j < DIMX; j++) {
        Serial.print(board[j][i]); 
      }
      Serial.println(" ");
    }
  }
}


// return smallest y-coordinate of a filled row, or -1 if none
int TetrisBoard::lowestFullRow() {
  for (int y = 0; y < DIMY; y++) {
    int count = 0;
    for (int x = 0; x < DIMX; x++) {
      count += (board[x][y] > 0) ? 1 : 0;  
    }
    if (count == DIMX) {
      return y;
    }
  }
  return(-1);
}

void TetrisBoard::rotTet(void)
{
  eraseTet();
  _tetRot = (_tetRot + 1) % 4;
  if (tetCollide()) {
    _tetRot = (_tetRot + 3) % 4;
  }
  adjustTet();
  if (tetCollide()) {
    _tetRot = (_tetRot + 3) % 4;
  }
  drawTet();
}

// return true iff tet position collides with board already in place
bool TetrisBoard::tetCollide() 
{
  if (tetMinY() < 0) {
    Serial.print(_tetType);
    Serial.print("=_tetType ");
    Serial.print(_tetRot);
    Serial.print("=_tetRot ");
    Serial.print(tetMinY());
    Serial.print("=minY ");
    Serial.print(_tetY);
    Serial.println("=_tetY  tetCollide()");
    return 1;
  }
  for (int x = _tetX; x < _tetX + 4; x++) {
    for (int y = _tetY; y < _tetY + 4; y++) {
      if (isOnBoard(x, y) && isInTet(x, y) && board[x][y] > 0) {
        if (VERBOSE) {
          Serial.print(x);
          Serial.print(",");
          Serial.print(y);
          Serial.print(" type=");
          Serial.print(_tetType);
          Serial.print(" rot=");
          Serial.print(_tetRot);
          Serial.print(" loc=");
          Serial.print(_tetX);
          Serial.print(",");
          Serial.print(_tetY);
          Serial.println("  collided!");
        }
        return 1;
      }
    }
  }
  return 0;
}

void TetrisBoard::dump(void)
{
  Serial.println("dump");
}

void TetrisBoard::killRow(int r) 
{
  Serial.print(r);
  Serial.println(" killRow");
  for (int y = r; y < DIMY - 1; y++) {
    for (int x = 0; x < DIMX; x++) {
      board[x][y] = board[x][y+1];
      paintBlock(x, y, board[x][y]);
    }
  }
}

bool TetrisBoard::spawn() 
{
  _tetX = random(DIMX - 4);
  _tetY = DIMY - 2;
  _tetType = random(7);
  _tetRot = 0;
  _tetColor = random(12) + 1;
  
  if (VERBOSE) Serial.println("spawn");
  if (tetCollide()) {
    Serial.println("immediate collide");
    return 0;
  }
  return 1;
}

void TetrisBoard::gameOver()
{
  Serial.println("Game Over!");
}



