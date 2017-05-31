/*
  Test.h - Test library for Wiring - description
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// ensure this library description is only included once
#ifndef TetrisBoard_h
#define TetrisBoard_h

#include <OctoWS2811.h>

#define LIGHT_LEVEL 5

#define DIMX 20
#define DIMY 40

#define COLS_LEDs 60 // all of the following params need to be adjusted for screen size
#define ROWS_LEDs 36  // LED_LAYOUT assumed 0 if ROWS_LEDs > 8
#define ROWS_PER_STRIP 12

#define STRIPS ((ROWS_LEDs+ROWS_PER_STRIP-1) / ROWS_PER_STRIP)
#define LEDS_PER_STRIP (COLS_LEDs*ROWS_PER_STRIP)
const int config = WS2811_GRB | WS2811_800kHz;

// library interface description
class TetrisBoard
{
  // user-accessible "public" interface
  public:
    TetrisBoard();
    void placeTet(int, int, int, int);
    void rotTet();
    void dump();
    bool moveTet(int, int);
    void freezeTet();
    void setupLEDs();
    void drawTet();
    void eraseTet();


  // library-accessible "private" interface
  private:
    int board[DIMX][DIMY];
    int tetMinX();
    int tetMaxX();
    int tetMinY();
    void adjustTet();
    bool isInTet(int, int);
    int _tetX, _tetY, _tetType, _tetRot, _tetColor;
    void setxy(int, int, uint32_t);
    int dimmer(int, int);
    int led_map(int, int);
    void paintTet(uint32_t);
    void drawBlock(int, int, uint32_t);
    bool tetCollide();
};

#endif

