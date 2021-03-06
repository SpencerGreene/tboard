/*
  Test.h - Test library for Wiring - description
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// ensure this library description is only included once
#ifndef TetrisBoard_h
#define TetrisBoard_h

#include <OctoWS2811.h>

#define LIGHT_LEVEL 8

#define DIMX 20
#define DIMY 24

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
    void rotTet();
    void dump();
    bool moveTet(int, int);
    void freezeTet();
    void setupGame();
    

    bool spawn();
    void gameOver();


  // library-accessible "private" interface
  private:
    // game state
    int board[DIMX][DIMY];
    int _tetX, _tetY, _tetType, _tetRot, _tetColor;

    // helper functions
    int tetMinX();
    int tetMaxX();
    int tetMinY();
    bool isInTet(int, int);
    bool isOnBoard(int, int);

    // game logic
    void adjustTet();
    bool tetCollide();
    int lowestFullRow();
    void killRow(int);

    // rendering
    int dimmer(int, int);
    int led_map(int, int);
    void setxy(int, int, uint32_t);
    void paintTet(uint32_t);
    void paintBlock(int, int, uint32_t);
    void drawTet();
    void eraseTet();
};

#endif

